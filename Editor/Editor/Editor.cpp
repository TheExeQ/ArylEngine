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

		std::string data = "Hello there how are you doing this fine day?";

		Ref<Aryl::NetPacket> packet = CreateRef<Aryl::NetPacket>();

		packet->header.id = Aryl::NetMessageType::StringMessage;

		packet->data.resize(data.size() + 1);
		memcpy_s(packet->data.data(), packet->data.size(), data.c_str(), packet->data.size());

		mySender->Send(packet, Aryl::IPv4Endpoint(Aryl::IPv4Address(sendAddress), sendPort));
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

		myReceiver = Aryl::UdpSocketReceiver::Create(builder->Build(), [](Aryl::NetPacket packet) {
			std::string str((const char*)packet.data.data());

			YL_CORE_TRACE(str);
			YL_CORE_TRACE("From {0}:{1}", packet.endpoint.GetAddress().GetAddressString(), packet.endpoint.GetPort());
			});
	}
	ImGui::SameLine();
	if (ImGui::Button("Print Info"))
	{
		YL_CORE_INFO("Address: {0}", hostAddress);
		YL_CORE_INFO("Port: {0}", hostPort);
	}

	ImGui::End();
	

	return false;
}
