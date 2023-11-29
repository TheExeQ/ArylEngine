#pragma once
#include "UdpSocketSender.h"

#include "Aryl/Core/Application.h"
#include "Aryl/Network/NetAPI.h"

#include "Platform/Network/Asio/Socket/AsioUdpSocketSender.h"

namespace Aryl
{
    extern NetReliableHandler g_ReliableFallback;
    extern std::mutex g_ReliableFallbackMutex;

    UdpSocketSender::UdpSocketSender(Ref<UdpSocket> socket)
        : mySocket(socket), myThread([this]() { Run(); })
    {
    }

    uint32_t UdpSocketSender::Run()
    {
        while (!myStopping)
        {
            auto start_time = std::chrono::high_resolution_clock::now();
            Update();

            std::lock_guard lock(g_ReliableFallbackMutex);

            for (auto& [fst, snd] : g_ReliableFallback.ReliableFallback)
            {
                auto& reliableFallbackVec = snd;
                for (auto it = reliableFallbackVec.begin(); it != reliableFallbackVec.end();)
                {
                    if (it->Retries <= 0)
                    {
                        it = reliableFallbackVec.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }

            auto end_time = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double> duration = end_time - start_time;
            const double delta_time_seconds = duration.count();

            for (auto& [fst, snd] : g_ReliableFallback.ReliableFallback)
            {
                for (auto& entry : snd)
                {
                    entry.Time -= delta_time_seconds;
                    if (entry.Time < 0.f)
                    {
                        YL_CORE_INFO("Resending reliable packet: {0}", entry.Packet->header.id);
                        Send(entry.Packet, entry.Endpoint, true);
                        entry.Time = myReliableTime;
                        entry.Retries--;
                    }
                }
            }
        }

        return 0;
    }

    void UdpSocketSender::Update()
    {
    }

    bool UdpSocketSender::Send(Ref<NetPacket> packet, IPv4Endpoint receiver, bool isReliablySent)
    {
        packet->header.size = packet->size();
        if (packet->size() > PACKET_MAXSIZE)
        {
            YL_CORE_WARN("Packet exceeded allowed size limit and will be aborted.");
            return false;
        }

        packet->endpoint = receiver;

        if (!myStopping && packet->endpoint.IsValid())
        {
            mySendQueue.push(packet);

            if (packet->header.packetType == NetPacketType::Reliable && !isReliablySent)
            {
                // YL_CORE_TRACE("Add: {0} (ID: {1})", receiver.ToString(), packet->header.id);
                g_ReliableFallback.ReliableFallback[receiver.ToString()].emplace_back(NetReliableEntry{
                    CreateRef<NetPacket>(*packet), receiver, myReliableTime, myReliableRetries
                });
            }
            return true;
        }

        return false;
    }

    Ref<UdpSocketSender> UdpSocketSender::Create(Ref<UdpSocket> socket)
    {
        switch (NetAPI::Current())
        {
        case NetAPIType::None: return nullptr;
        case NetAPIType::Asio: return CreateRef<AsioUdpSocketSender>(socket);
        case NetAPIType::WinSock2: return nullptr;
        }

        YL_CORE_ASSERT(false, "Unknown NetAPI");
        return nullptr;
    }
}
