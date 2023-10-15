#pragma once

#include "HostSettings.h"
#include "Aryl/Network/Socket/UdpSocketReceiver.h"
#include "Aryl/Network/Socket/UdpSocketSender.h"

namespace Aryl
{
    class Host
    {
    public:
        Host(HostSettings hostSettings);
        ~Host();

        bool SendMessage(Ref<NetPacket> packet);

        IPv4Endpoint Endpoint;
        
    protected:
        void HandleMessage(NetPacket packet);

    private:
        Ref<UdpSocketSender> mySender = nullptr;
        Ref<UdpSocketReceiver> myReceiver = nullptr;
    };
}