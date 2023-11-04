#include "Client.h"

#include <Aryl/Scene/Scene.h>
#include <Aryl/Scene/Entity.h>
#include <Aryl/Scene/SceneManager.h>
#include <Aryl/Components/Components.h>

namespace Aryl
{
    Client::Client(HostSettings hostSettings)
        : Host(hostSettings, YL_BIND_EVENT_FN(Client::HandleMessage))
    {
    }

    Client::~Client()
    {
        if (myConnectionsMap.size() > 0)
        {
            const Ref<NetPacket> connectPacket = CreateRef<NetPacket>();
            connectPacket->header.messageType = NetMessageType::Disconnect;
            
            SendMessage(connectPacket);
        }
    }

    void Client::Connect(const IPv4Endpoint& endpoint)
    {
        Host::Connect(endpoint);

        {
            myConnectionsMap[endpoint.ToString()] = NetConnection({endpoint, 0});

            const Ref<NetPacket> connectPacket = CreateRef<NetPacket>();
            connectPacket->header.messageType = NetMessageType::Connect;

            (*connectPacket) << myReceiver->GetSocket()->GetEndpoint().GetPort();

            SendMessage(connectPacket);
        }
    }

    void Client::HandleMessage(NetPacket& packet)
    {
        Host::HandleMessage(packet);
        static int imageVariation = 1;

        if (packet.header.messageType == NetMessageType::SyncWorld)
        {
            auto& registry = SceneManager::GetActiveScene()->GetRegistry();

            size_t entityCount, spriteCount;
            packet >> entityCount;
            packet >> spriteCount;

            registry.clear();

            for (int i = 0; i < entityCount; ++i)
            {
                registry.create();
            }

            for (int i = 0; i < spriteCount; ++i)
            {
                uint32_t ent;
                float x, y, z;

                packet >> ent;
                packet >> x;
                packet >> y;
                packet >> z;

                registry.emplace_or_replace<SpriteRendererComponent>((entt::entity)ent,
                                                                     std::string("test") + std::to_string(
                                                                         ent % imageVariation) + ".png");
                registry.emplace_or_replace<TransformComponent>((entt::entity)ent, glm::vec3(x, y, z), glm::quat(),
                                                                glm::vec3(1.f));

                // YL_INFO("{0}: [{1}, {2}, {3}]", ent, x, y, z);
            }
        }

        if (packet.header.messageType == NetMessageType::CreateEntity)
        {
            auto& registry = SceneManager::GetActiveScene()->GetRegistry();
            auto newEntity = Entity(registry.create(), SceneManager::GetActiveScene().get());

            registry.emplace<TransformComponent>(newEntity.GetId()).position = {0.f, 0.f, 0.f};
            registry.emplace<SpriteRendererComponent>(newEntity.GetId()).spritePath = std::string("test") +
                std::to_string(((uint32_t)newEntity.GetId()) % imageVariation) + ".png";
        }

        if (packet.header.messageType == NetMessageType::RemoveEntity)
        {
            uint32_t entId;
            packet >> entId;
            
            auto& registry = SceneManager::GetActiveScene()->GetRegistry();
            registry.remove<SpriteRendererComponent>((entt::entity)entId);
        }

        if (packet.header.messageType == NetMessageType::SyncEntity)
        {
            uint32_t ent;
            float x, y, z;

            packet >> ent;
            packet >> x;
            packet >> y;
            packet >> z;

            auto& registry = SceneManager::GetActiveScene()->GetRegistry();
            registry.emplace_or_replace<TransformComponent>((entt::entity)ent, glm::vec3(x, y, z), glm::quat(),
                                                            glm::vec3(1.f));
        }
    }
}
