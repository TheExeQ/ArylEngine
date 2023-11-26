#pragma once
#include "Host.h"

namespace Aryl
{
    class Client : public Host
    {
    public:
        Client(HostSettings hostSettings);
        ~Client();

        void Connect(const IPv4Endpoint& endpoint) override;
        void PingServer();
        
    protected:
        void HandleMessage(NetPacket& packet) override;
    };
}
