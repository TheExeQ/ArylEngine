#pragma once

#include "Aryl/Network/NetPacket.h"
#include "Aryl/Network/Interfaces/IPv4/IPv4Endpoint.h"

namespace Aryl
{
    struct HostSettings
    {
        uint32_t preferredPort = 0u;
    };

    struct NetReliableEntry
    {
        Ref<NetPacket> Packet;
        IPv4Endpoint Endpoint;
        float Time = 2.f;
        int Retries = 3;
    };

    struct NetReliableHandler
    {
        std::mutex Mutex;
        std::unordered_map<std::string, std::vector<NetReliableEntry>> ReliableFallback;
    };

    struct NetConnection
    {
        IPv4Endpoint listenEndpoint;
        uint32_t sendId = 0;
        uint32_t receiveId = 0;
    };
}
