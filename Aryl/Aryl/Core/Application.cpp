#include "Application.h"

#include "Aryl/Scene/Entity.h"
#include "Aryl/Scene/Scene.h"
#include "Aryl/Scene/SceneManager.h"
#include "Aryl/Components/Components.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

namespace Aryl
{
	Application::Application(const ApplicationInfo& info)
	{
		YL_CORE_ASSERT(!myInstance, "Application already exists!");

		myInfo = info;
		myInstance = this;

		myIsRunning = true;
		myIsMinimized = false;

		Log::Initialize();
		WindowProperties windowProperties;
		windowProperties.width = info.width;
		windowProperties.height = info.height;
		windowProperties.vsync = info.useVSync;
		windowProperties.title = info.title;
		windowProperties.windowMode = info.windowMode;

		myWindow = Window::Create(windowProperties);
		myWindow->SetEventCallback(YL_BIND_EVENT_FN(Application::OnEvent));

		// #SAMUEL_TODO: Temporary Stuff
		{
			glfwMakeContextCurrent(myWindow->GetNativeWindow());
			int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			YL_CORE_ASSERT(status, "Failed to initialize Glad!");
			glfwSwapInterval(1); // Enable vsync

			SceneManager::SetActiveScene(CreateRef<Scene>("Test Scene"));
		}

		if (info.enableImGui)
		{
			myImGuiImplementation = ImGuiImplementation::Create();
		}
	}

	Application::~Application()
	{
		Log::Shutdown();
		myLayerStack.Clear();

		myImGuiImplementation = nullptr;
		myWindow = nullptr;
		myInstance = nullptr;
	}

	void Application::Run()
	{
		while (myIsRunning)
		{
			// Begin Frame
			myWindow->BeginFrame();

			float time = (float)glfwGetTime();
			myCurrentDeltaTime = time - myLastTotalTime;
			myLastTotalTime = time;

			// Update
			{
				AppUpdateEvent updateEvent(myCurrentDeltaTime);
				OnEvent(updateEvent);
			}

			// Render
			{
				static ImVec4 clear_color = ImVec4(0.2f, 0.2f, 0.20f, 1.00f);
				int display_w, display_h;
				glfwGetFramebufferSize(myWindow->GetNativeWindow(), &display_w, &display_h);
				glViewport(0, 0, display_w, display_h);
				glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
				glClear(GL_COLOR_BUFFER_BIT);

				AppRenderEvent renderEvent;
				OnEvent(renderEvent);
			}

			if (myInfo.enableImGui)
			{
				myImGuiImplementation->Begin();

				ImGui::Begin("TestWindow");
				if (ImGui::Button("Spawn Ent"))
				{
					auto entity = SceneManager::GetActiveScene()->CreateEntity("New");
					auto comp = entity.GetComponent<TagComponent>();
					YL_TRACE(comp.tag);
					comp.tag = "Yolo kid";
					YL_TRACE(comp.tag);
				}
				ImGui::End();

				AppImGuiUpdateEvent imguiEvent{};
				OnEvent(imguiEvent);

				myImGuiImplementation->End();
			}

			// Present
			myWindow->Present();
		}
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(YL_BIND_EVENT_FN(Application::OnWindowCloseEvent));
		dispatcher.Dispatch<WindowResizeEvent>(YL_BIND_EVENT_FN(Application::OnWindowResizeEvent));

		for (auto layer : myLayerStack)
		{
			layer->OnEvent(event);
			if (event.handled)
			{
				break;
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		myLayerStack.PushLayer(layer);
	}

	void Application::PopLayer(Layer* layer)
	{
		myLayerStack.PopLayer(layer);
	}

	bool Application::OnWindowCloseEvent(WindowCloseEvent&)
	{
		myIsRunning = false;
		return false;
	}

	bool Application::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			myIsMinimized = true;
			return false;
		}

		myIsMinimized = false;
		myWindow->Resize(e.GetWidth(), e.GetHeight());
		return false;
	}
}