#include "Host.h"

#include "Client.h"
#include "Server.h"
#include "Aryl/Core/Application.h"
#include "Aryl/Network/Socket/UdpSocketBuilder.h"

namespace Aryl
{
    NetReliableHandler s_ReliableFallback;
    
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

            auto socket = builder->Build();

            if (socket)
            {
                myReceiver = UdpSocketReceiver::Create(socket, handleMessageDelegate);
                YL_CORE_TRACE("Starting UDP receiver on {0}:{1}", hostEndpoint.GetAddress().ToString(),
                              hostEndpoint.GetPort());
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
    }

    bool Host::SendMessage(const Ref<NetPacket>& packet)
    {
        if (mySender && myConnectionsMap.find(myEndpoint.ToString()) != myConnectionsMap.end())
        {
            NetConnection& connection = myConnectionsMap.at(myEndpoint.ToString());
            packet->header.id = connection.sendId;

            auto ep = myConnectionsMap.at(myEndpoint.ToString()).listenEndpoint;
            YL_INFO("Sending ID:{0} msg_type:{1} to {2}", packet->header.id, (int)packet->header.messageType, ep.ToString());
            
            if (mySender->Send(packet, ep))
            {
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
            const Ref<NetPacket> ackPacket = CreateRef<NetPacket>();
            ackPacket->header.messageType = NetMessageType::Acknowledgement;

            (*ackPacket) << packet.header.id;
            
            SendMessage(ackPacket);
            YL_CORE_TRACE("Acknowledgement (ID: {0})", packet.header.id);
        }
    }
}
