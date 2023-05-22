#include "Application.h"

#include <glfw/glfw3.h>

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

		myNetManager = NetManager::Create();

		if (!myInfo.headless)
		{
			myWindow = Window::Create(windowProperties);
			myWindow->SetEventCallback(YL_BIND_EVENT_FN(Application::OnEvent));

			if (myInfo.enableImGui)
			{
				myImGuiImplementation = ImGuiImplementation::Create();
			}
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
		YL_PROFILE_THREAD("Main");
		while (myIsRunning)
		{
			YL_PROFILE_FRAME("Frame");

			float time = (float)glfwGetTime();
			Timestep timestep = time - myLastFrameTime;
			myLastFrameTime = time;

			// Update
			{
				YL_PROFILE_SCOPE("Application::Update");

				AppUpdateEvent updateEvent(timestep);
				OnEvent(updateEvent);
			}

			if (!myInfo.headless)
			{
				// Begin Frame
				myWindow->BeginFrame();

				// Render
				{
					YL_PROFILE_SCOPE("Application::Render");
					AppRenderEvent renderEvent;
					OnEvent(renderEvent);
				}

				if (myInfo.enableImGui)
				{
					YL_PROFILE_SCOPE("Application::ImGui");
					myImGuiImplementation->Begin();

					AppImGuiUpdateEvent imguiEvent{};
					OnEvent(imguiEvent);

					myImGuiImplementation->End();
				}

				// Present
				myWindow->Present();
			}
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