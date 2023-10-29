﻿#pragma once

#include "HostSettings.h"
#include "Aryl/Network/Socket/UdpSocketReceiver.h"
#include "Aryl/Network/Socket/UdpSocketSender.h"

namespace Aryl
{
    struct NetConnection
    {
        IPv4Endpoint listenEndpoint;
        uint32_t sendId = 0;
    };
    
    class Host
    {
    public:
        Host(HostSettings hostSettings, const std::function<void(NetPacket)>& handleMessageDelegate);
        ~Host();

        bool SendMessage(const Ref<NetPacket>& packet);
        virtual void Connect(const IPv4Endpoint& endpoint);

    protected:
        virtual void HandleMessage(NetPacket& packet);
        
        std::unordered_map<std::string, NetConnection> myConnectionsMap;
        IPv4Endpoint myEndpoint;

        Ref<UdpSocketSender> mySender = nullptr;
        Ref<UdpSocketReceiver> myReceiver = nullptr;
    };
}