#include "NetworkTesting.h"

#include <Aryl/Core/Base.h>
#include "Aryl/Core/Random.h"
#include <Aryl/Core/Application.h>

#include <Aryl/Components/Components.h>
#include <Aryl/Scene/Entity.h>
#include <Aryl/Scene/Scene.h>
#include <Aryl/Scene/SceneManager.h>

#include <Aryl/Events/KeyEvent.h>
#include <Aryl/Input/KeyCodes.h>

#include <ImGui.h>
#include <entt/entt.hpp>

#include "Aryl/Network/Host/Client.h"
#include "Aryl/Network/Host/Host.h"
#include "Aryl/Network/Host/Server.h"
#include "Platform/Network/Asio/AsioContext.h"

NetworkTesting::NetworkTesting()
{
    if (Aryl::Application::Get().IsHeadless())
    {
        Aryl::HostSettings hostSettings;
        hostSettings.preferredPort = 44000;
        Aryl::Application::Get().GetNetworkContext()->InitServer(hostSettings);
    }
    else
    {
        Aryl::Application::Get().GetNetworkContext()->InitClient();
    }

    // ReflectionTesting();
}

NetworkTesting::~NetworkTesting()
{
}

void NetworkTesting::ReflectionTesting()
{
    // ENTT REFLECTION TESTING

    auto& registry = Aryl::SceneManager::GetActiveScene()->GetRegistry();
    myTestingEntity = Aryl::Entity(registry.create(), Aryl::SceneManager::GetActiveScene().get());
    registry.emplace<Aryl::TestComponent>(myTestingEntity.GetId());

    Aryl::RegisterReflection();

    auto& comp = myTestingEntity.GetComponent<Aryl::TestComponent>();

    YL_CORE_TRACE(comp.intValue);
    YL_CORE_TRACE(comp.floatValue);
    YL_CORE_TRACE(comp.boolValue);

    auto type = entt::resolve(entt::hashed_string(typeid(Aryl::TestComponent).name()));
    if (type)
    {
        auto data = type.data(entt::hashed_string("intValue"));
        auto data2 = type.data(entt::hashed_string("floatValue"));
        auto data3 = type.data(entt::hashed_string("boolValue"));

        auto type_id = entt::type_id<Aryl::TestComponent>().hash();
        auto storage = registry.storage(type_id);

        YL_CORE_TRACE("TypeId: {0}", type_id);

        if (storage)
        {
            auto compontentPtr = storage->get(myTestingEntity.GetId());
            auto componentAny = type.from_void(compontentPtr);

            if (data) { data.set(componentAny, 5); }
            if (data2) { data2.set(componentAny, 4.3f); }
            if (data3) { data3.set(componentAny, true); }
        }
    }

    YL_CORE_TRACE(comp.intValue);
    YL_CORE_TRACE(comp.floatValue);
    YL_CORE_TRACE(comp.boolValue);
}

void NetworkTesting::OnEvent(Aryl::Event& e)
{
    Aryl::EventDispatcher dispatcher(e);

    dispatcher.Dispatch<Aryl::AppUpdateEvent>(YL_BIND_EVENT_FN(NetworkTesting::OnUpdate));
    dispatcher.Dispatch<Aryl::AppImGuiUpdateEvent>(YL_BIND_EVENT_FN(NetworkTesting::OnImGuiUpdate));
}

bool NetworkTesting::OnUpdate(Aryl::AppUpdateEvent& e)
{
    if (Aryl::Application::Get().IsHeadless())
    {
        constexpr double max_timer = 5;
        static double timer = max_timer;

        timer -= e.GetTimestep();
        if (timer < 0)
        {
            Aryl::Application::Get().GetNetworkContext()->GetServer()->CreateEntity();
            timer = max_timer;
        }
    }

    Aryl::Host* host;
    if (Aryl::Application::Get().IsHeadless())
    {
        host = Aryl::Application::Get().GetNetworkContext()->GetServer().get();
    }
    else
    {
        host = Aryl::Application::Get().GetNetworkContext()->GetClient().get();
    }

    if (host)
    {
        std::lock_guard lock(host->myEnttMutex);

        auto& registry = Aryl::SceneManager::GetActiveScene()->GetRegistry();
        auto entities = registry.view<Aryl::ObjectMovement>();

        std::vector<entt::entity> deleteEnts;

        for (auto ent : entities)
        {
            auto& comp = registry.get<Aryl::ObjectMovement>(ent);
            comp.current_lerp_time += e.GetTimestep();
            if (comp.current_lerp_time > comp.lerp_time)
            {
                Aryl::Random::SetSeed(
                    comp.start.x + comp.start.y + comp.start.z + comp.target.x + comp.target.y + comp.target.z);

                const float targetX = Aryl::Random::GetRandomFloat(-1.f, 1.f);
                const float targetY = Aryl::Random::GetRandomFloat(-1.f, 1.f);

                comp.start = comp.target;
                comp.target = glm::vec3(750.f * targetX, 500.f * targetY, 0.f);
                comp.current_lerp_time = 0.f;
            }
            registry.get<Aryl::TransformComponent>(ent).position = glm::mix(
                comp.start, comp.target, comp.current_lerp_time / comp.lerp_time);

            for (auto ent2 : entities)
            {
                if (ent != ent2)
                {
                    float distance = glm::distance(registry.get<Aryl::TransformComponent>(ent).position,
                                                   registry.get<Aryl::TransformComponent>(ent2).position);

                    if (distance <= 100.0f)
                    {
                        deleteEnts.emplace_back(ent);
                        deleteEnts.emplace_back(ent2);
                    }
                }
            }
        }

        if (Aryl::Application::Get().IsHeadless() && !deleteEnts.empty())
        {
            reinterpret_cast<Aryl::Server*>(host)->RemoveEntities(deleteEnts);
        }
    }

    return false;
}

bool NetworkTesting::OnImGuiUpdate(Aryl::AppImGuiUpdateEvent& e)
{
    ArylNetExample();
    return false;
}

void NetworkTesting::ArylNetExample()
{
    static char serverIp[16] = "127.0.0.1";
    static int serverPort = 44000;
    static bool hasConnected = false;

    ImGui::Begin("ArylNet");
    ImGui::InputText("Server Address", serverIp, sizeof(serverIp));
    ImGui::InputInt("Server Port", &serverPort);

    auto client = Aryl::Application::Get().GetNetworkContext()->GetClient();
    if (ImGui::Button("Connect") && !hasConnected)
    {
        client->Connect(Aryl::IPv4Endpoint(Aryl::IPv4Address(serverIp), serverPort));
        hasConnected = true;
    }
    ImGui::End();

    auto& sendStats = client->GetSenderStats();

    ImGui::Begin("Net Stats");
    ImGui::Text("Packet Loss: %.2f%%", sendStats.PacketLoss);
    ImGui::Text("Bits Sent This Frame: %d", sendStats.BitsSentThisFrame);
    ImGui::End();

    if (!myTestingEntity.IsNull())
    {
        auto& comp = myTestingEntity.GetComponent<Aryl::TestComponent>();

        ImGui::Begin("Example Component");
        ImGui::InputInt("ComponentInt", &comp.intValue);
        ImGui::InputFloat("ComponentFloat", &comp.floatValue);
        ImGui::Checkbox("ComponentBool", &comp.boolValue);
        if (ImGui::Button("Sync"))
        {
            // Notify server/client of changes
        }
        ImGui::End();
    }
}
