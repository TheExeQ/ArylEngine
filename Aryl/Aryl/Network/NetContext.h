#pragma once

#include "Aryl/Core/Base.h"
#include "Host/HostSettings.h"

namespace Aryl
{
    class Server;
    class Client;

    class NetContext
    {
    public:
        NetContext() = default;
        virtual ~NetContext() = default;

        virtual void Init() = 0;

        Ref<Server> InitServer(HostSettings hostSettings = HostSettings());
        Ref<Client> InitClient(HostSettings hostSettings= HostSettings());

         Ref<Server> GetServer() { return myServer; };
         Ref<Client> GetClient() { return myClient; };

        static Ref<NetContext> Create();

     private:
         Ref<Server> myServer = nullptr;
         Ref<Client> myClient = nullptr;
    };
}
