#include "Server.h"

#include <Aryl/Scene/Scene.h>
#include <Aryl/Scene/Entity.h>
#include <Aryl/Scene/SceneManager.h>
#include <Aryl/Components/Components.h>

namespace Aryl
{
    Server::Server(HostSettings hostSettings)
        : Host(hostSettings, YL_BIND_EVENT_FN(Server::HandleMessage))
    {
    }

    Server::~Server()
    {
    }

    void Server::Connect(const IPv4Endpoint& endpoint)
    {
        Host::Connect(endpoint);
    }

    void Server::CreateEntity()
    {
        std::lock_guard lock(myEnttMutex);
        auto& registry = SceneManager::GetActiveScene()->GetRegistry();
        {
            auto newEntity = Entity(registry.create(), SceneManager::GetActiveScene().get());

            glm::vec3 start, target;
            float lerpTime = 2.f;
            start = {0.f, 0.f, 0.f};
            target = {500.f, 0.f, 0.f};

            registry.emplace<TransformComponent>(newEntity.GetId()).position = start;
            registry.emplace<ObjectMovement>(newEntity.GetId()) = {start, target, 0.f, lerpTime};

            const Ref<NetPacket> newEntPacket = CreateRef<NetPacket>();
            newEntPacket->header.messageType = NetMessageType::CreateEntity;

            (*newEntPacket) << lerpTime;
            (*newEntPacket) << target.z << target.y << target.x;
            (*newEntPacket) << start.z << start.y << start.x;

            MulticastPacket(newEntPacket);
        }
    }

    void Server::RemoveEntities(std::vector<entt::entity> entities)
    {
    }

    void Server::MulticastPacket(Ref<NetPacket> packet, const IPv4Endpoint* ignoreEP)
    {
        for (auto connection : myConnectionsMap)
        {
            uint32_t splitPoint = connection.first.find(':') + 1;
            std::string p_str = connection.first.substr(splitPoint, connection.first.size() - splitPoint);
            uint32_t p = atoi(p_str.c_str());

            IPv4Endpoint ep(connection.second.listenEndpoint.GetAddress(), p);
            if (ignoreEP && ep.ToString() == ignoreEP->ToString()) { continue; }
            Connect(ep);
            SendMessage(packet);
        }
    }

    void Server::HandleMessage(NetPacket& packet)
    {
        Host::HandleMessage(packet);

        std::lock_guard lock(myEnttMutex);
        auto& registry = SceneManager::GetActiveScene()->GetRegistry();

        if (packet.header.messageType == NetMessageType::Connect)
        {
            uint32_t port;
            packet >> port;
            IPv4Endpoint clientListenEndpoint(packet.endpoint.GetAddress(), port);
            myConnectionsMap[packet.endpoint.ToString()] = NetConnection({clientListenEndpoint, 0});

            // Sync World
            {
                const Ref<NetPacket> syncPacket = CreateRef<NetPacket>();
                syncPacket->header.messageType = NetMessageType::SyncWorld;

                auto spriteView = registry.view<SpriteRendererComponent>();

                for (auto ent : spriteView)
                {
                    auto transform = registry.get<TransformComponent>(ent);
                    (*syncPacket) << transform.position.z << transform.position.y << transform.position.x;
                    (*syncPacket) << ent;
                }

                (*syncPacket) << spriteView.size();
                (*syncPacket) << registry.size();

                Connect(packet.endpoint);
                SendMessage(syncPacket);
            }
        }
    }
}
