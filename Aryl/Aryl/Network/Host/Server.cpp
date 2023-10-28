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

    void Server::HandleMessage(const NetPacket& packet)
    {
        Host::HandleMessage(packet);

        if (packet.header.messageType == NetMessageType::Connect)
        {
            static float xPos = 0.f;
            static int connectionCount = 0;
            static int imageVariation = 1;
            connectionCount++;
            xPos += 100.f;
            
            auto& registry = SceneManager::GetActiveScene()->GetRegistry();
            {
                auto newEntity = Entity(registry.create(), SceneManager::GetActiveScene().get());
            
                registry.emplace<TransformComponent>(newEntity.GetId()).position = { xPos, 0.f, 0.f };
                registry.emplace<SpriteRendererComponent>(newEntity.GetId()).spritePath = std::string("test") + std::to_string(connectionCount % imageVariation) + ".png";
            }
        }
    }
}
