#pragma once

#include "HostSettings.h"
#include "Aryl/Network/Socket/UdpSocketReceiver.h"
#include "Aryl/Network/Socket/UdpSocketSender.h"

namespace Aryl
{
    struct NetStatistics
    {
        int64_t ping = 0;
        float packetLoss = 0.f;
        int bitsSent = 0;
        int bitsReceived = 0;
        
        std::unordered_map<uint32_t, std::chrono::steady_clock::time_point> pingAckIds;
        int packetsLostCount = 0;
    };

    class Host
    {
    public:
        Host(HostSettings hostSettings, const std::function<void(NetPacket)>& handleMessageDelegate);
        ~Host();

        bool SendMessage(const Ref<NetPacket>& packet);
        virtual void Connect(const IPv4Endpoint& endpoint);

        const NetStatistics& GetStats() const { return myNetStats; }

        std::mutex myEnttMutex;

    protected:
        virtual void HandleMessage(NetPacket& packet);

        bool ShouldProcessMessage(NetPacket& packet);
        void OnPacketLost(uint32_t id);

        void SendAck(NetPacket& packet);

        std::unordered_map<std::string, NetConnection> myConnectionsMap;
        IPv4Endpoint myEndpoint;

        NetStatistics myNetStats;
        std::mutex myNetStatsMutex;

        std::vector<uint32_t> myMissedIds;
        
        Ref<UdpSocketSender> mySender = nullptr;
        Ref<UdpSocketReceiver> myReceiver = nullptr;
    };
}
