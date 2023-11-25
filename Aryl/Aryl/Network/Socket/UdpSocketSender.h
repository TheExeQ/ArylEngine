#pragma once
#include "Aryl/Core/Base.h"
#include "Aryl/Network/Socket/UdpSocket.h"

#include "Aryl/Network/NetPacket.h"
#include "Aryl/Network/Interfaces/IPv4/IPv4Address.h"
#include "Aryl/Network/Interfaces/IPv4/IPv4Endpoint.h"

#include <queue>

namespace Aryl
{
    struct SenderStats
    {
        int BitsSentThisFrame = 0;
    };

    class UdpSocketSender
    {
    public:
        UdpSocketSender(Ref<UdpSocket> socket);
        virtual ~UdpSocketSender() { myThread.join(); };

        void Stop()
        {
            myStopping = true;
        }

        bool Send(Ref<NetPacket> packet, IPv4Endpoint receiver, bool isReliablySent = false);
        Ref<UdpSocket> GetSocket() { return mySocket; };
        const SenderStats& GetStats() const { return myStats; };

        static Ref<UdpSocketSender> Create(Ref<UdpSocket> socket);

    protected:
        virtual void Update();

        SenderStats myStats;
        std::queue<Ref<NetPacket>> mySendQueue;
        int myLostPacketsCount = 0;
        int myReliableRetries = 3;
        float myReliableTime = 5.f;

        bool myStopping = false;
        std::thread myThread;

        Ref<UdpSocket> mySocket = nullptr;

    private:
        uint32_t Run();

        std::unordered_map<std::string, uint32_t> myPacketIdMap;
    };
}
