#pragma once
#include "Host.h"

namespace Aryl
{
    class Client : public Host
    {
    public:
        Client(HostSettings hostSettings);
        ~Client();
        
    protected:
        void HandleMessage(const NetPacket& packet) override;
    };
}
