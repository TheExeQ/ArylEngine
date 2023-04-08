#include "Editor.h"

#include <Aryl/Core/Base.h>
#include <Aryl/Core/Application.h>

#include <Aryl/Components/Components.h>
#include <Aryl/Scene/Entity.h>
#include <Aryl/Scene/Scene.h>
#include <Aryl/Scene/SceneManager.h>

#include <Aryl/Events/KeyEvent.h>
#include <Aryl/Input/KeyCodes.h>

#include <ImGui.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <entt/entt.hpp>

#include <Aryl/Networking/Socket/UdpSocketBuilder.h>

Editor::Editor()
{
	YL_ASSERT(!myInstance, "Editor already exists!");
	myInstance = this;
}

Editor::~Editor()
{
	myInstance = nullptr;
}

void Editor::OnAttach()
{
	myTestingEntity = Aryl::SceneManager::GetActiveScene()->CreateEntity("TestEntity");
	myTestingEntity.AddComponent<Aryl::TestComponent>();

	if (Aryl::Application::Get().IsHeadless())
	{
		char address[16] = "0.0.0.0";
		uint32_t port = 44000;

		Ref<Aryl::UdpSocketBuilder> builder = Aryl::UdpSocketBuilder::Create(Aryl::Application::Get().GetNetworkContext());
		builder->BoundToAddress(Aryl::IPv4Address(address));
		builder->BoundToPort(port);

		YL_CORE_TRACE("Starting UDP server on {0}:{1}", address, port);

		myReceiver = Aryl::UdpSocketReceiver::Create(builder->Build(), [](Aryl::NetPacket packet) {
			std::string str((const char*)packet.data.data());

			YL_CORE_TRACE(str + " from {0}:{1}", packet.endpoint.GetAddress().GetAddressString(), packet.endpoint.GetPort());
			});

		builder = Aryl::UdpSocketBuilder::Create(Aryl::Application::Get().GetNetworkContext());
		builder->BoundToAddress(Aryl::IPv4Address(address));
		builder->BoundToPort(0);

		mySender = Aryl::UdpSocketSender::Create(builder->Build());

		std::string data = "Ping";

		Ref<Aryl::NetPacket> packet = CreateRef<Aryl::NetPacket>();

		packet->header.id = Aryl::NetMessageType::StringMessage;

		packet->data.resize(data.size() + 1);
		std::memcpy(packet->data.data(), data.c_str(), packet->data.size());

		mySender->Send(packet, Aryl::IPv4Endpoint(Aryl::IPv4Address("192.168.1.168"), port));
	}

	// ENTT REFLECTION TESTING
	{
		//auto& registry = Aryl::SceneManager::GetActiveScene()->GetRegistry();

		//entt::entity entity = registry.create();
		//std::string component_class_name = "my_type";
		//std::string component_member_name = "value";
		//std::string component_member_type = "float";
		//float component_member_data = 21.4f;

		//auto type = entt::resolve(entt::hashed_string(component_class_name.c_str()));
		//auto member = type.data(entt::hashed_string(component_member_name.c_str()));

		//registry.emplace<my_type>(entity, 42.0f);

		//auto component_id = entt::type_id<my_type>();

		//auto component_meta_type = entt::resolve<my_type>();

		//auto& component_data = registry.get_or_emplace<entt::any>(entity, my_type{});

		//auto& my_type_data = entt::any_cast<my_type&>(component_data);
		//my_type_data.value = 13.37f;

		//auto value = my_type_data.value;
		//YL_CORE_TRACE("Value: {0}", value);

		// ---

		//entt::meta<my_type>()
		//	.type(entt::hashed_string("my_type"))
		//	.data<&my_type::value>(entt::hashed_string("value"));

		//registry.emplace<entt::meta_any>(myTestingEntity.GetId(), my_type{ 42.0f });

		//auto type = entt::resolve(entt::hashed_string("my_type"));
		//auto data = type.data(entt::hashed_string("value"));
		//auto& any = registry.get<entt::meta_any>(myTestingEntity.GetId());
		//
		//if (auto* inst = any.try_cast<my_type>())
		//{
		//	inst->value = 20.f;
		//}

		//auto& comp = registry.get<entt::meta_any>(myTestingEntity.GetId());
		//if (auto* inst = comp.try_cast<my_type>(); inst)
		//{
		//	YL_CORE_TRACE("Value: {0}", inst->value);
		//}
		//
		//auto pool = registry.storage(type.id());
		//data.set(any.cast(type), 5.3f);
	}
}

void Editor::OnDetach()
{

}

void Editor::OnEvent(Aryl::Event& e)
{
	Aryl::EventDispatcher dispatcher(e);

	dispatcher.Dispatch<Aryl::AppRenderEvent>(YL_BIND_EVENT_FN(Editor::OnRender));
	dispatcher.Dispatch<Aryl::AppImGuiUpdateEvent>(YL_BIND_EVENT_FN(Editor::OnImGuiUpdate));
}

bool Editor::OnRender(Aryl::AppRenderEvent& e)
{
	static ImVec4 clear_color = ImVec4(0.2f, 0.2f, 0.20f, 1.00f);
	int display_w, display_h;
	glfwGetFramebufferSize(Aryl::Application::Get().GetWindow().GetNativeWindow(), &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);

	return false;
}

bool Editor::OnImGuiUpdate(Aryl::AppImGuiUpdateEvent& e)
{
	ArylNetExample();

	return false;
}

void Editor::ArylNetExample()
{
	static char hostAddress[16] = "127.0.0.1";
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
		if (mySender)
		{
			mySender->Stop();
			mySender = nullptr;
		}

		Ref<Aryl::UdpSocketBuilder> builder = Aryl::UdpSocketBuilder::Create(Aryl::Application::Get().GetNetworkContext());
		builder->BoundToAddress(Aryl::IPv4Address(hostAddress));
		builder->BoundToPort(0);

		mySender = Aryl::UdpSocketSender::Create(builder->Build());
	}

	ImGui::SameLine();
	if (ImGui::Button("Receiver"))
	{
		if (myReceiver)
		{
			myReceiver->Stop();
			myReceiver = nullptr;
		}

		Ref<Aryl::UdpSocketBuilder> builder = Aryl::UdpSocketBuilder::Create(Aryl::Application::Get().GetNetworkContext());
		builder->BoundToAddress(Aryl::IPv4Address(hostAddress));
		builder->BoundToPort(hostPort);
		currentHostPort = hostPort;

		myReceiver = Aryl::UdpSocketReceiver::Create(builder->Build(), [](Aryl::NetPacket packet) {
			std::string str((const char*)packet.data.data());

			YL_CORE_TRACE(str + " from {0}:{1}", packet.endpoint.GetAddress().GetAddressString(), packet.endpoint.GetPort());
			});
	}

	if (mySender)
	{
		ImGui::SameLine();
		if (ImGui::Button("Ping"))
		{
			std::string data = "Ping";

			Ref<Aryl::NetPacket> packet = CreateRef<Aryl::NetPacket>();

			packet->header.id = Aryl::NetMessageType::StringMessage;

			packet->data.resize(data.size() + 1);
			std::memcpy(packet->data.data(), data.c_str(), packet->data.size());

			mySender->Send(packet, Aryl::IPv4Endpoint(Aryl::IPv4Address(sendAddress), sendPort));
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
