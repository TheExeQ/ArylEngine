#pragma once

#include "HostSettings.h"
#include "Aryl/Network/Socket/UdpSocketReceiver.h"
#include "Aryl/Network/Socket/UdpSocketSender.h"

namespace Aryl
{
    class Host
    {
    public:
        Host(HostSettings hostSettings, const std::function<void(NetPacket)>& handleMessageDelegate);
        ~Host();

        bool SendMessage(const Ref<NetPacket>& packet) const;
        void Connect(const IPv4Endpoint& endpoint);

        // IPv4Endpoint GetListenEndpoint();
        
    protected:
        virtual void HandleMessage(const NetPacket& packet);
        IPv4Endpoint myEndpoint;

    private:
        Ref<UdpSocketSender> mySender = nullptr;
        Ref<UdpSocketReceiver> myReceiver = nullptr;
    };
}