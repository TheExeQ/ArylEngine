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
	static std::string hostAddress = "127.0.0.1";
	static int hostPort = 44000;
	static int currentHostPort = hostPort;

	static std::string sendAddress = "127.0.0.1";
	static int sendPort = 44000;

	ImGui::Begin("ArylNet");
	ImGui::InputInt("Host Port", &hostPort);
	ImGui::NewLine();
	ImGui::InputText("Send Address", (char*)sendAddress.c_str(), 16);
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
			memcpy_s(packet->data.data(), packet->data.size(), data.c_str(), packet->data.size());

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

	return false;
}
