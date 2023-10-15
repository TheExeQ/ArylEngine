#include "NetworkTesting.h"

#include <Aryl/Core/Base.h>
#include <Aryl/Core/Application.h>

#include <Aryl/Components/Components.h>
#include <Aryl/Scene/Entity.h>
#include <Aryl/Scene/Scene.h>
#include <Aryl/Scene/SceneManager.h>

#include <Aryl/Events/KeyEvent.h>
#include <Aryl/Input/KeyCodes.h>

#include <Aryl/Network/Socket/UdpSocketBuilder.h>

#include <ImGui.h>
#include <entt/entt.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Aryl/Network/Host/Client.h"
#include "Aryl/Network/Host/Host.h"

NetworkTesting::NetworkTesting()
{
	// ENTT REFLECTION TESTING
	if (true)
	{
		Aryl::HostSettings hostSettings;
		hostSettings.preferredPort = 44000;
		
		Aryl::Application::Get().GetNetworkContext()->InitServer(hostSettings);
		auto client = Aryl::Application::Get().GetNetworkContext()->InitClient();

		client->Endpoint = Aryl::IPv4Endpoint(Aryl::IPv4Address("127.0.0.1"), hostSettings.preferredPort);
		
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
}

NetworkTesting::~NetworkTesting()
{
}

void NetworkTesting::OnEvent(Aryl::Event& e)
{
	Aryl::EventDispatcher dispatcher(e);

	dispatcher.Dispatch<Aryl::AppImGuiUpdateEvent>(YL_BIND_EVENT_FN(NetworkTesting::OnImGuiUpdate));
}

bool NetworkTesting::OnImGuiUpdate(Aryl::AppImGuiUpdateEvent& e)
{
	ArylNetExample();
	return false;
}

void NetworkTesting::ArylNetExample()
{
	static char hostAddress[16] = "127.0.0.1";
	static char currentHostAddress[16] = "127.0.0.1";
	static int hostPort = 44000;
	static int currentHostPort = hostPort;

	static char sendAddress[16] = "127.0.0.1";
	static int sendPort = 44000;

	ImGui::Begin("ArylNet");
	ImGui::InputText("Host Address", hostAddress, sizeof(hostAddress));
	ImGui::InputInt("Host Port", &hostPort);
	ImGui::NewLine();
	ImGui::InputText("Send Address", sendAddress, sizeof(sendAddress));
	ImGui::InputInt("Send Port", &sendPort);

	if (ImGui::Button("Sender"))
	{
		SetupSender(Aryl::IPv4Endpoint(Aryl::IPv4Address(hostAddress), 0));
	}

	ImGui::SameLine();
	if (ImGui::Button("Receiver"))
	{
		SetupReceiver(Aryl::IPv4Endpoint(Aryl::IPv4Address(hostAddress), hostPort), [this](Aryl::NetPacket packet) {
			if (packet.header.id == Aryl::NetMessageType::StringMessage)
			{
				std::string str((const char*)packet.data.data());
				YL_CORE_TRACE(str + " from {0}:{1}", packet.endpoint.GetAddress().GetAddressString(), packet.endpoint.GetPort());
			}
			});
	}

	//if (mySender)
	{
		ImGui::SameLine();
		if (ImGui::Button("Ping"))
		{
			std::string data = "Ping";

			Ref<Aryl::NetPacket> packet = CreateRef<Aryl::NetPacket>();

			packet->header.id = Aryl::NetMessageType::StringMessage;

			packet->data.resize(data.size() + 1);
			std::memcpy(packet->data.data(), data.c_str(), packet->data.size());

			Aryl::Application::Get().GetNetworkContext()->GetClient()->SendMessage(packet);
			
			//mySender->Send(packet, Aryl::IPv4Endpoint(Aryl::IPv4Address(sendAddress), sendPort));
		}
	}

	ImGui::NewLine();

	// Debug info

	if (myReceiver || mySender)
	{
		ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "Connected");
		if (myReceiver)
		{
			std::string text = "Receiver Active on port: " + std::to_string(currentHostPort);
			ImGui::TextColored({ 0.f, 0.5f, 0.f, 1.f }, text.c_str());
		}
		if (mySender)
		{
			ImGui::TextColored({ 0.f, 0.5f, 0.f, 1.f }, "Sender Active");
		}
	}
	else
	{
		ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, "Disconnected");
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

void NetworkTesting::SetupSender(Aryl::IPv4Endpoint endpoint)
{
	return;
	if (mySender)
	{
		mySender->Stop();
		mySender = nullptr;
	}

	Ref<Aryl::UdpSocketBuilder> builder = Aryl::UdpSocketBuilder::Create(Aryl::Application::Get().GetNetworkContext());
	builder->BoundToAddress(endpoint.GetAddress());
	builder->BoundToPort(0);

	auto socket = builder->Build();

	if (socket)
	{
		mySender = Aryl::UdpSocketSender::Create(socket);
	}
}

void NetworkTesting::SetupReceiver(Aryl::IPv4Endpoint endpoint, std::function<void(Aryl::NetPacket)> callback)
{
	return;
	if (myReceiver)
	{
		bool hasSender = mySender != nullptr;
		if (!hasSender)
		{
			Ref<Aryl::UdpSocketBuilder> builder = Aryl::UdpSocketBuilder::Create(Aryl::Application::Get().GetNetworkContext());
			builder->BoundToAddress(endpoint.GetAddress());
			builder->BoundToPort(0);

			mySender = Aryl::UdpSocketSender::Create(builder->Build());
		}
		myReceiver->Stop();

		Ref<Aryl::NetPacket> packet = CreateRef<Aryl::NetPacket>();
		packet->header.id = Aryl::NetMessageType::Disconnect;

		mySender->Send(packet, endpoint);

		myReceiver = nullptr;

		if (!hasSender)
		{
			mySender->Stop();
			mySender = nullptr;
		}
	}

	Ref<Aryl::UdpSocketBuilder> builder = Aryl::UdpSocketBuilder::Create(Aryl::Application::Get().GetNetworkContext());
	builder->BoundToAddress(endpoint.GetAddress());
	builder->BoundToPort(endpoint.GetPort());

	auto socket = builder->Build();

	if (socket)
	{
		myReceiver = Aryl::UdpSocketReceiver::Create(socket, callback);
		YL_CORE_TRACE("Starting UDP server on {0}:{1}", endpoint.GetAddress().GetAddressString(), endpoint.GetPort());
	}
}
