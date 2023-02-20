#include "ylpch.h"
#include "Application.h"

#include <iostream>

namespace Aryl
{
	Application::Application()
	{
		myIsRunning = true;
		myIsMinimized = false;

		Log::Initialize();

		WindowProperties windowProperties;

		myWindow = Window::Create(windowProperties);
		myWindow->SetEventCallback(YL_BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application()
	{
		Log::Shutdown();

		myLayerStack.Clear();
		myWindow->Shutdown();

		myWindow = nullptr;
		myInstance = nullptr;
	}

	void Application::Run()
	{
		while (myIsRunning)
		{
			// Update

			// Begin Frame
			myWindow->BeginFrame();

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