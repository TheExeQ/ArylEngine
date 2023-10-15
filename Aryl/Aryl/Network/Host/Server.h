#pragma once
#include "Host.h"

namespace Aryl
{
    class Server : public Host
    {
    public:
        Server(HostSettings hostSettings);
        ~Server();
    };
}
