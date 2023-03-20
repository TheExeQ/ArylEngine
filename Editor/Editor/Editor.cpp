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
	Ref<Aryl::UdpSocketBuilder> builder = Aryl::UdpSocketBuilder::Create(Aryl::Application::Get().GetNetworkContext());
	builder->BoundToAddress(Aryl::IPv4Address("127.0.0.1"));
	builder->BoundToPort(44000);

	mySocket = builder->Build();
	mySocket->Start();
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
	ImGui::Begin("TestWindow");
	if (ImGui::Button("Spawn Ent"))
	{
		if (auto scene = Aryl::SceneManager::GetActiveScene())
		{
			auto entity = scene->CreateEntity("New");
			auto comp = entity.GetComponent<Aryl::TagComponent>();
			YL_CORE_TRACE(comp.tag);
			comp.tag = "Rename";
			YL_CORE_TRACE(comp.tag);
		}
	}
	ImGui::End();

	return false;
}
