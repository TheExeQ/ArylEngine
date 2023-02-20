#include "Application.h"

#include <iostream>

namespace Aryl
{
	Application::Application()
	{
		myIsRunning = true;

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
			YL_CORE_INFO("APP::Update()");

			// Begin Frame
			myWindow->BeginFrame();

			// Present
			myWindow->Present();
		}
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

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
}