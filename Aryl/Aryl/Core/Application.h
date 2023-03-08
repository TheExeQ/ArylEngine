#pragma once

#include "Aryl/Core/Base.h"

#include "Aryl/Core/Window.h"
#include "Aryl/Core/Layer/Layer.h"
#include "Aryl/Core/Layer/LayerStack.h"

#include "Aryl/Events/ApplicationEvent.h"
#include "Aryl/Log/Log.h"

#include "Aryl/ImGui/ImGuiImplementation.h"

#include "Aryl/Networking/NetManager.h"

#include <glfw/glfw3.h>

#include <string>
#include <filesystem>

namespace Aryl
{
	struct ApplicationInfo
	{
		ApplicationInfo(const std::string& aTitle = "Aryl", WindowMode aWindowMode = WindowMode::Windowed, uint32_t aWidth = 1280, uint32_t aHeight = 720, bool aUseVSync = true, bool aEnableImGui = true)
			: title(aTitle), width(aWidth), height(aHeight), useVSync(aUseVSync), enableImGui(aEnableImGui), windowMode(aWindowMode)
		{
		}

		std::string title;
		WindowMode windowMode;
		uint32_t width;
		uint32_t height;
		bool useVSync;
		bool enableImGui;
		bool headless = false;
		bool isRuntime = false;

		std::string version = "1.0";
	};

	class Application
	{
	public:
		Application(const ApplicationInfo& info = ApplicationInfo());
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

		float myCurrentDeltaTime = 0.f;
		float myLastTotalTime = 0.f;

		LayerStack myLayerStack;

		Scope<Window> myWindow = nullptr;
		Scope<ImGuiImplementation> myImGuiImplementation;
		Scope<NetManager> myNetManager;

		ApplicationInfo myInfo;
		inline static Application* myInstance;
	};

	static Application* CreateApplication(const std::filesystem::path& appPath);
}