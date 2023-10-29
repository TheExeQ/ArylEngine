#pragma once
#include "Host.h"

namespace Aryl
{
    class Server : public Host
    {
    public:
        Server(HostSettings hostSettings);
        ~Server();

        void Connect(const IPv4Endpoint& endpoint) override;
        
    protected:
        void HandleMessage(NetPacket& packet) override;
    };
}
