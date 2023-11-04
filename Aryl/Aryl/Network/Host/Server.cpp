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

    void Server::HandleMessage(NetPacket& packet)
    {
        Host::HandleMessage(packet);

        static std::unordered_map<std::string, entt::entity> clientActors;

        if (packet.header.messageType == NetMessageType::Disconnect && clientActors.find(packet.endpoint.ToString()) != clientActors.end())
        {
            myConnectionsMap.erase(packet.endpoint.ToString());
            
            const Ref<NetPacket> newEntPacket = CreateRef<NetPacket>();
            newEntPacket->header.messageType = NetMessageType::RemoveEntity;

            (*newEntPacket) << ((uint32_t)clientActors[packet.endpoint.ToString()]);
            
            for (auto connection : myConnectionsMap)
            {
                uint32_t splitPoint = connection.first.find(':') + 1;
                std::string p_str = connection.first.substr(splitPoint, connection.first.size() - splitPoint);
                uint32_t p = atoi(p_str.c_str());

                IPv4Endpoint ep(connection.second.listenEndpoint.GetAddress(), p);
                if (ep.ToString() == packet.endpoint.ToString()) { continue; }
                Connect(ep);
                SendMessage(newEntPacket);
            }
        }
        
        if (packet.header.messageType == NetMessageType::Connect)
        {
            static int imageVariation = 2;

            uint32_t port;
            packet >> port;
            IPv4Endpoint clientListenEndpoint(packet.endpoint.GetAddress(), port);
            myConnectionsMap[packet.endpoint.ToString()] = NetConnection({clientListenEndpoint, 0});

            auto& registry = SceneManager::GetActiveScene()->GetRegistry();
            {
                auto newEntity = Entity(registry.create(), SceneManager::GetActiveScene().get());

                registry.emplace<TransformComponent>(newEntity.GetId()).position = {0.f, 0.f, 0.f};
                registry.emplace<SpriteRendererComponent>(newEntity.GetId()).spritePath = std::string("test") +
                    std::to_string(((uint32_t)newEntity.GetId()) % imageVariation) + ".png";

                clientActors[packet.endpoint.ToString()] = newEntity.GetId();

                const Ref<NetPacket> newEntPacket = CreateRef<NetPacket>();
                newEntPacket->header.messageType = NetMessageType::CreateEntity;

                for (auto connection : myConnectionsMap)
                {
                    uint32_t splitPoint = connection.first.find(':') + 1;
                    std::string p_str = connection.first.substr(splitPoint, connection.first.size() - splitPoint);
                    uint32_t p = atoi(p_str.c_str());

                    IPv4Endpoint ep(connection.second.listenEndpoint.GetAddress(), p);
                    if (ep.ToString() == packet.endpoint.ToString()) { continue; }
                    Connect(ep);
                    SendMessage(newEntPacket);
                }
            }

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

        if (packet.header.messageType == NetMessageType::PlayerMovement)
        {
            float x, y;
            packet >> x;
            packet >> y;

            auto& registry = SceneManager::GetActiveScene()->GetRegistry();
            entt::entity clientEntity = clientActors.at(packet.endpoint.ToString());
            auto& pos = registry.get<TransformComponent>(clientEntity).position;
            pos.x += x;
            pos.y += y;

            const Ref<NetPacket> syncPacket = CreateRef<NetPacket>();
            syncPacket->header.messageType = NetMessageType::SyncEntity;

            (*syncPacket) << pos.z << pos.y << pos.x;
            (*syncPacket) << clientEntity;

            for (auto connection : myConnectionsMap)
            {
                uint32_t splitPoint = connection.first.find(':') + 1;
                std::string p_str = connection.first.substr(splitPoint, connection.first.size() - splitPoint);
                uint32_t p = atoi(p_str.c_str());

                IPv4Endpoint ep(connection.second.listenEndpoint.GetAddress(), p);
                Connect(ep);
                SendMessage(syncPacket);
            }
        }
    }
}
