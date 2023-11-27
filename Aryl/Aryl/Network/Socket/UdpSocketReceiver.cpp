#pragma once
#include "UdpSocketReceiver.h"

#include "Aryl/Network/NetAPI.h"

#include "Platform/Network/Asio/Socket/AsioUdpSocketReceiver.h"

namespace Aryl
{
    extern NetReliableHandler g_ReliableFallback;
    extern std::mutex g_ReliableFallbackMutex;

    UdpSocketReceiver::UdpSocketReceiver(Ref<UdpSocket> socket, std::function<void(NetPacket)> onDataReceivedDelegate)
        : mySocket(socket), myThread([this]() { Run(); }), myOnDataReceivedDelegate(onDataReceivedDelegate)
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

                if (packet.header.messageType == NetMessageType::Acknowledgement)
                {
                    std::lock_guard lock(g_ReliableFallbackMutex);

                    uint32_t id;
                    packet >> id;
                    packet << id;
                    
                    auto idMatches = [id](const NetReliableEntry& entry)
                    {
                        return entry.Packet && entry.Packet->header.id == id;
                    };

                    auto it = std::find_if(g_ReliableFallback.ReliableFallback.begin(),
                                           g_ReliableFallback.ReliableFallback.end(), idMatches);

                    if (it != g_ReliableFallback.ReliableFallback.end())
                    {
                        g_ReliableFallback.ReliableFallback.erase(it);
                    }
                    YL_CORE_TRACE("Received Acknowledgement (ID: {0})", id);
                }

                packet.endpoint = sender;

                myOnDataReceivedDelegate(packet);
            }

            Update();
        }

        return 0;
    }

    Ref<UdpSocketReceiver> UdpSocketReceiver::Create(Ref<UdpSocket> socket,
                                                     std::function<void(NetPacket)> onDataReceivedDelegate)
    {
        switch (NetAPI::Current())
        {
        case NetAPIType::None: return nullptr;
        case NetAPIType::Asio: return CreateRef<AsioUdpSocketReceiver>(socket, onDataReceivedDelegate);
        case NetAPIType::WinSock2: return nullptr;
        }

        YL_CORE_ASSERT(false, "Unknown NetAPI");
        return nullptr;
    }
}
