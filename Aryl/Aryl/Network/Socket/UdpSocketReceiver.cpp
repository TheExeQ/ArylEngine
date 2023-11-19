#pragma once
#include "UdpSocketReceiver.h"

#include "Aryl/Network/NetAPI.h"

#include "Platform/Network/Asio/Socket/AsioUdpSocketReceiver.h"

namespace Aryl
{
    extern NetReliableHandler s_ReliableFallback;

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
                    std::lock_guard lock(s_ReliableFallback.Mutex);

                    uint32_t id;
                    packet >> id;
                    
                    auto idMatches = [id](const NetReliableEntry& entry)
                    {
                        return entry.Packet->header.id == id;
                    };

                    auto it = std::find_if(s_ReliableFallback.ReliableFallback.begin(),
                                           s_ReliableFallback.ReliableFallback.end(), idMatches);

                    if (it != s_ReliableFallback.ReliableFallback.end())
                    {
                        s_ReliableFallback.ReliableFallback.erase(it);
                    }
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
