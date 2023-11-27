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
            myConnectionsMap[endpoint.ToString()] = NetConnection({endpoint});

            const Ref<NetPacket> connectPacket = CreateRef<NetPacket>();
            connectPacket->header.messageType = NetMessageType::Connect;
            connectPacket->header.packetType = NetPacketType::Reliable;

            (*connectPacket) << myReceiver->GetSocket()->GetEndpoint().GetPort();

            SendMessage(connectPacket);
        }
    }

    void Client::PingServer()
    {
        const Ref<NetPacket> pingPacket = CreateRef<NetPacket>();
        pingPacket->header.messageType = NetMessageType::Ping;
        pingPacket->header.packetType = NetPacketType::Reliable;
        SendMessage(pingPacket);
    }

    void Client::HandleMessage(NetPacket& packet)
    {
        if (!ShouldProcessMessage(packet)) { return; }

        static int imageVariation = 2;

        if (packet.header.messageType == NetMessageType::SyncWorld)
        {
            std::lock_guard lock(myEnttMutex);
            auto& registry = SceneManager::GetActiveScene()->GetRegistry();

            size_t entityCount, objCount;
            packet >> entityCount;
            packet >> objCount;

            registry.clear();

            for (int i = 0; i < entityCount; ++i)
            {
                registry.create();
            }

            for (int i = 0; i < objCount; ++i)
            {
                uint32_t ent;
                glm::vec3 pos, start, target;
                float curLerp, targetLerp;

                packet >> ent;
                packet >> pos.x >> pos.y >> pos.z;

                packet >> start.x >> start.y >> start.z;
                packet >> target.x >> target.y >> target.z;
                packet >> curLerp >> targetLerp;

                registry.emplace_or_replace<SpriteRendererComponent>(static_cast<entt::entity>(ent),
                                                                     std::string("test") + std::to_string(
                                                                         ent % imageVariation) + ".png");
                registry.emplace_or_replace<TransformComponent>(static_cast<entt::entity>(ent), pos, glm::quat(),
                                                                glm::vec3(0.5f));

                registry.emplace_or_replace<ObjectMovement>(static_cast<entt::entity>(ent), start, target, curLerp,
                                                            targetLerp);
            }
        }

        if (packet.header.messageType == NetMessageType::CreateEntity)
        {
            std::lock_guard lock(myEnttMutex);
            auto& registry = SceneManager::GetActiveScene()->GetRegistry();
            auto newId = registry.create();

            entt::entity ent;
            glm::vec3 start, target;
            float lerpTime;

            packet >> ent;
            myServerToClientEntityMap[static_cast<uint32_t>(ent)] = static_cast<uint32_t>(newId);

            packet >> start.x >> start.y >> start.z;
            packet >> target.x >> target.y >> target.z;
            packet >> lerpTime;

            registry.emplace<TransformComponent>(newId) = TransformComponent{start, glm::quat(), {0.5f, 0.5f, 0.5f}};
            registry.emplace<ObjectMovement>(newId) = ObjectMovement{start, target, 0.f, lerpTime};
            registry.emplace<SpriteRendererComponent>(newId).spritePath = std::string("test") +
                std::to_string(static_cast<uint32_t>(ent) % imageVariation) + ".png";
        }

        if (packet.header.messageType == NetMessageType::RemoveEntity)
        {
            std::lock_guard lock(myEnttMutex);
            size_t entCount;
            packet >> entCount;

            auto& registry = SceneManager::GetActiveScene()->GetRegistry();
            for (int i = 0; i < entCount; ++i)
            {
                uint32_t entId;
                packet >> entId;

                if (myServerToClientEntityMap.find(entId) != myServerToClientEntityMap.end())
                {
                    auto clientId = myServerToClientEntityMap.at(entId);

                    registry.remove<SpriteRendererComponent>(static_cast<entt::entity>(clientId));
                    registry.remove<ObjectMovement>(static_cast<entt::entity>(clientId));

                    myServerToClientEntityMap.erase(entId);
                }
                else
                {
                    myQueuedDelete.emplace_back(entId);
                }
            }

            ProcessDeletions(myServerToClientEntityMap, myQueuedDelete);
        }

        Host::HandleMessage(packet);
    }

    void Client::ProcessDeletions(std::unordered_map<uint32_t, uint32_t>& map, std::vector<uint32_t>& vec)
    {
        for (auto it = vec.begin(); it != vec.end();)
        {
            if (map.find(*it) != map.end())
            {
                auto clientId = map.at(*it);

                auto& registry = SceneManager::GetActiveScene()->GetRegistry();
                registry.remove<SpriteRendererComponent>(static_cast<entt::entity>(clientId));
                registry.remove<ObjectMovement>(static_cast<entt::entity>(clientId));
                
                map.erase(*it);

                it = vec.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}
