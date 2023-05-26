#include "Editor.h"

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
	myNetworkTest = CreateRef<NetworkTesting>();
	myRendererTest = CreateRef<RendererTesting>();
}

void Editor::OnDetach()
{
}

void Editor::OnEvent(Aryl::Event& e)
{
	Aryl::EventDispatcher dispatcher(e);

	myNetworkTest->OnEvent(e);
	myRendererTest->OnEvent(e);
}
