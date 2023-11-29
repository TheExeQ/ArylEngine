#pragma once
#include "UdpSocketReceiver.h"

#include "Aryl/Network/NetAPI.h"

#include "Platform/Network/Asio/Socket/AsioUdpSocketReceiver.h"

namespace Aryl
{
    UdpSocketReceiver::UdpSocketReceiver(Ref<UdpSocket> socket, std::function<void(NetPacket)> onDataReceivedDelegate, Ref<NetReliableHandler> nrh)
        : mySocket(socket), myThread([this]() { Run(); }), myOnDataReceivedDelegate(onDataReceivedDelegate), reliableFallback(nrh)
    {
    }

    uint32_t UdpSocketReceiver::Run()
    {
        while (!myStopping)
        {
            if (myOnDataReceivedDelegate)
            {
                PacketBuffer buffer;
                IPv4Endpoint sender;

                mySocket->ReceieveFrom(buffer, sender);

                NetPacket packet;
                packet.deserialize(buffer);

                if (packet.header.messageType == NetMessageType::Acknowledgement && reliableFallback)
                {
                    std::lock_guard lock(reliableFallback->Mutex);

                    uint32_t id, rPort;
                    packet >> id;
                    packet >> rPort;
                    packet << rPort;
                    packet << id;

                    IPv4Endpoint rEP(sender.GetAddress(), rPort);

                    auto idMatches = [id, rEP](const NetReliableEntry& entry)
                    {
                        return entry.Packet && entry.Packet->header.id == id && entry.Packet->endpoint.ToString() == rEP
                            .ToString();
                    };

                    auto& vec = reliableFallback->ReliableFallback.at(rEP.ToString());

                    if (auto it = std::find_if(vec.begin(),
                                               vec.end(), idMatches); it != vec.end())
                    {
                        // YL_CORE_TRACE("Del: {0} (ID: {1})", rEP.ToString(), id);
                        vec.erase(it);
                    }
                    // YL_CORE_TRACE("Received Acknowledgement (ID: {0})", id);
                }

                packet.endpoint = sender;

                myOnDataReceivedDelegate(packet);
            }

            Update();
        }

        return 0;
    }

    Ref<UdpSocketReceiver> UdpSocketReceiver::Create(Ref<UdpSocket> socket,
                                                     std::function<void(NetPacket)> onDataReceivedDelegate, Ref<NetReliableHandler> nrh)
    {
        switch (NetAPI::Current())
        {
        case NetAPIType::None: return nullptr;
        case NetAPIType::Asio: return CreateRef<AsioUdpSocketReceiver>(socket, onDataReceivedDelegate, nrh);
        case NetAPIType::WinSock2: return nullptr;
        }

        YL_CORE_ASSERT(false, "Unknown NetAPI");
        return nullptr;
    }
}
