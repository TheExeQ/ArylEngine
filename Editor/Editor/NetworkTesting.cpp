#include "NetworkTesting.h"

#include <Aryl/Core/Base.h>
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

	if (ImGui::Button("Connect") && !hasConnected)
	{
		auto client = Aryl::Application::Get().GetNetworkContext()->GetClient();
		client->Connect(Aryl::IPv4Endpoint(Aryl::IPv4Address(serverIp), serverPort));
		hasConnected = true;
	}
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
