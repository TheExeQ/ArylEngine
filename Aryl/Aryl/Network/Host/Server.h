#pragma once
#include <entt/entt.hpp>

#include "Host.h"

namespace Aryl
{
    class Server : public Host
    {
    public:
        Server(HostSettings hostSettings);
        ~Server();

        void Connect(const IPv4Endpoint& endpoint) override;

        void CreateEntity();
        void RemoveEntities(std::vector<entt::entity> entities);
        
    protected:
        void HandleMessage(NetPacket& packet) override;
        void MulticastPacket(Ref<NetPacket> packet, const IPv4Endpoint* ignoreEP = nullptr);
    };
}
