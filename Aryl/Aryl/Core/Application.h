#pragma once

#include "Aryl/Core/Base.h"

#include "Aryl/Core/Window.h"
#include "Aryl/Core/Layer/Layer.h"
#include "Aryl/Core/Layer/LayerStack.h"

#include "Aryl/Events/ApplicationEvent.h"
#include "Aryl/Log/Log.h"

#include <string>
#include <filesystem>

namespace Aryl
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		inline Window& GetWindow() const { return *myWindow; }
		inline static Application& Get() { return *myInstance; }

	private:
		bool OnWindowCloseEvent(WindowCloseEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);

		bool myIsRunning = false;
		bool myIsMinimized = false;

		LayerStack myLayerStack;

		Scope<Window> myWindow = nullptr;

		inline static Application* myInstance;
	};

	static Application* CreateApplication(const std::filesystem::path& appPath);
}