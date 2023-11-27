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
            if (registry.view<ObjectMovement>().size() > 9) return;

            auto newEntity = Entity(registry.create(), SceneManager::GetActiveScene().get());

            bool alter = (uint32_t)newEntity.GetId() % 2 == 0;
            glm::vec3 start, target;
            float lerpTime = (alter) ? 4.f : 2.f;
            start = {0.f, 0.f, 0.f};
            target = {(alter) ? 500.f : -500.f, 0.f, 0.f};

            registry.emplace<TransformComponent>(newEntity.GetId()) = {start, glm::quat(), {0.5f, 0.5f, 0.5f}};
            registry.emplace<ObjectMovement>(newEntity.GetId()) = {start, target, 0.f, lerpTime};

            const Ref<NetPacket> newEntPacket = CreateRef<NetPacket>();
            newEntPacket->header.messageType = NetMessageType::CreateEntity;
            newEntPacket->header.packetType = NetPacketType::Reliable;

            (*newEntPacket) << lerpTime;
            (*newEntPacket) << target.z << target.y << target.x;
            (*newEntPacket) << start.z << start.y << start.x;
            
            (*newEntPacket) << static_cast<uint32_t>(newEntity.GetId());

            // YL_INFO("Create new entity, current count: {0}", registry.view<ObjectMovement>().size());
            
            MulticastPacket(newEntPacket);
        }
    }

    void Server::RemoveEntities(std::vector<entt::entity> entities)
    {
        const Ref<NetPacket> delEntPacket = CreateRef<NetPacket>();
        delEntPacket->header.messageType = NetMessageType::RemoveEntity;
        delEntPacket->header.packetType = NetPacketType::Reliable;

        auto& registry = SceneManager::GetActiveScene()->GetRegistry();
        for (auto ent : entities)
        {
            registry.destroy(ent);
            
            (*delEntPacket) << ent;
        }

        (*delEntPacket) << entities.size();

        MulticastPacket(delEntPacket);

        // YL_INFO("Remove {0} entities", entities.size());
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
        Connect(packet.endpoint);
        if (!ShouldProcessMessage(packet)) { return; }
        
        std::lock_guard lock(myEnttMutex);
        auto& registry = SceneManager::GetActiveScene()->GetRegistry();

        if (packet.header.messageType == NetMessageType::Connect)
        {
            uint32_t port;
            packet >> port;
            IPv4Endpoint clientListenEndpoint(packet.endpoint.GetAddress(), port);
            myConnectionsMap[packet.endpoint.ToString()] = NetConnection({clientListenEndpoint});

            // Sync World
            {
                const Ref<NetPacket> syncPacket = CreateRef<NetPacket>();
                syncPacket->header.messageType = NetMessageType::SyncWorld;
                syncPacket->header.packetType = NetPacketType::Reliable;

                auto entities = registry.view<ObjectMovement>();

                for (auto ent : entities)
                {
                    auto transform = registry.get<TransformComponent>(ent);
                    auto movement = registry.get<ObjectMovement>(ent);

                    (*syncPacket) << movement.lerp_time << movement.current_lerp_time;
                    (*syncPacket) << movement.target.z << movement.target.y << movement.target.x;
                    (*syncPacket) << movement.start.z << movement.start.y << movement.start.x;
                    
                    (*syncPacket) << transform.position.z << transform.position.y << transform.position.x;
                    (*syncPacket) << ent;
                }

                (*syncPacket) << entities.size();
                (*syncPacket) << registry.size();

                Connect(packet.endpoint);
                SendMessage(syncPacket);
            }
        }

        Host::HandleMessage(packet);
    }
}
