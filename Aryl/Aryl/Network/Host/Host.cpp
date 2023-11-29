#include "Host.h"

#include "Aryl/Core/Application.h"
#include "Aryl/Network/Socket/UdpSocketBuilder.h"

namespace Aryl
{
    NetReliableHandler g_ReliableFallback;
    std::mutex g_ReliableFallbackMutex;

    Host::Host(HostSettings hostSettings, const std::function<void(NetPacket)>& handleMessageDelegate)
    {
        if (!mySender)
        {
            Ref<UdpSocketBuilder> builder = UdpSocketBuilder::Create(Application::Get().GetNetworkContext());
            builder->BoundToAddress(IPv4Address("0.0.0.0"));
            builder->BoundToPort(0);

            auto socket = builder->Build();

            if (socket)
            {
                mySender = UdpSocketSender::Create(socket);
            }
        }

        if (!myReceiver)
        {
            Ref<UdpSocketBuilder> builder = UdpSocketBuilder::Create(Application::Get().GetNetworkContext());

            IPv4Endpoint hostEndpoint = IPv4Endpoint(IPv4Address("0.0.0.0"), hostSettings.preferredPort);

            builder->BoundToAddress(hostEndpoint.GetAddress());
            builder->BoundToPort(hostEndpoint.GetPort());

            const auto socket = builder->Build();
            const auto socketEP = socket->GetEndpoint();

            if (socket)
            {
                myReceiver = UdpSocketReceiver::Create(socket, handleMessageDelegate);
                YL_CORE_TRACE("Starting UDP receiver on {0}:{1}", socketEP.GetAddress().ToString(),
                              socketEP.GetPort());
            }
        }
    }

    Host::~Host()
    {
        if (myReceiver)
        {
            myReceiver->Stop();

            if (mySender)
            {
                Ref<NetPacket> packet = CreateRef<NetPacket>();
                packet->header.messageType = NetMessageType::Disconnect;

                SendMessage(packet);
            }

            myReceiver = nullptr;
        }

        if (mySender)
        {
            mySender->Stop();
            mySender = nullptr;
        }

        std::unique_lock lock(myEnttMutex, std::try_to_lock);
        std::unique_lock lock2(myNetStatsMutex, std::try_to_lock);

        if (!lock.owns_lock())
        {
            lock.lock();
        }

        if (!lock2.owns_lock())
        {
            lock2.lock();
        }
    }

    bool Host::SendMessage(const Ref<NetPacket>& packet)
    {
        if (mySender && myConnectionsMap.find(myEndpoint.ToString()) != myConnectionsMap.end())
        {
            NetConnection& connection = myConnectionsMap.at(myEndpoint.ToString());
            packet->header.id = connection.sendId;

            if (const auto ep = connection.listenEndpoint; mySender->Send(packet, ep))
            {
                // YL_INFO("Sending ID:{0} msg_type:{1} to {2}", packet->header.id, static_cast<int>(packet->header.messageType),
                //         ep.ToString());

                std::lock_guard lock(myNetStatsMutex);
                myNetStats.bitsSent += (sizeof(packet->header) + packet->data.size()) * 8;

                if (packet->header.packetType == NetPacketType::Reliable)
                {
                    myNetStats.pingAckIds[packet->header.id] = std::chrono::high_resolution_clock::now();
                }

                connection.sendId++;
            }
        }
        return false;
    }

    void Host::Connect(const IPv4Endpoint& endpoint)
    {
        myEndpoint = endpoint;
    }

    void Host::HandleMessage(NetPacket& packet)
    {
        {
            std::lock_guard lock(myNetStatsMutex);
            myNetStats.bitsReceived += (sizeof(packet.header) + packet.size()) * 8;

            if (packet.header.messageType == NetMessageType::Acknowledgement)
            {
                uint32_t id;
                packet >> id;

                if (const auto it = myNetStats.pingAckIds.find(id); it != myNetStats.pingAckIds.end())
                {
                    myNetStats.ping = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::high_resolution_clock::now() - it->second
                    ).count();
                    myNetStats.pingAckIds.erase(it);
                }
            }
        }

        // TESTING
        if (packet.header.messageType == NetMessageType::Connect)
        {
            YL_CORE_TRACE("New connection from {0} (ID: {1})", packet.endpoint.ToString(), packet.header.id);
        }
        if (packet.header.messageType == NetMessageType::Disconnect)
        {
            YL_CORE_TRACE("{0} disconnected (ID: {1})", packet.endpoint.ToString(), packet.header.id);
        }
        if (packet.header.packetType == NetPacketType::Reliable)
        {
            SendAck(packet);
        }
    }

    bool Host::ShouldProcessMessage(NetPacket& packet)
    {
        if (myConnectionsMap.find(myEndpoint.ToString()) == myConnectionsMap.end())
        {
            return true;
        }

        NetConnection& connection = myConnectionsMap.at(myEndpoint.ToString());
        if (packet.header.id > connection.receiveId)
        {
            const auto missedIds = packet.header.id - connection.receiveId - 1u;
            for (int i = 0; i < missedIds; ++i)
            {
                uint32_t missedId = packet.header.id - (i + 1);
                myMissedIds.emplace_back(missedId);
                YL_CORE_WARN("Missed packet: {0}", missedId);
            }
            connection.receiveId = packet.header.id;
            return true;
        }
        else if (const auto it = std::find(myMissedIds.begin(), myMissedIds.end(), packet.header.id); it != myMissedIds.
            end())
        {
            myMissedIds.erase(it);
            YL_CORE_WARN("Recovered packet: {0}", packet.header.id);
            return true;
        }

        SendAck(packet);
        YL_CORE_WARN("Ignoring packet (Already registered): {0}", packet.header.id);
        return false;
    }

    void Host::OnPacketLost(uint32_t id)
    {
        myNetStats.packetsLostCount++;
        myNetStats.packetLoss = myNetStats.packetsLostCount / id;
    }

    void Host::SendAck(NetPacket& packet)
    {
        const Ref<NetPacket> ackPacket = CreateRef<NetPacket>();
        ackPacket->header.messageType = NetMessageType::Acknowledgement;

        (*ackPacket) << myReceiver->GetSocket()->GetEndpoint().GetPort();
        (*ackPacket) << packet.header.id;

        SendMessage(ackPacket);
        // YL_CORE_TRACE("Sending Acknowledgement (ID: {0})", packet.header.id);
    }
}
