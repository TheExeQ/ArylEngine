#pragma once

#include "Aryl/Events/Event.h"
#include "Aryl/Core/Base.h"

#include <functional>
#include <filesystem>
#include <glm/glm.hpp>

struct GLFWwindow;

namespace Aryl
{
	enum class WindowMode : uint32_t
	{
		Windowed = 0,
		Fullscreen,
		Borderless
	};

	struct WindowProperties
	{
		WindowProperties(const std::string& aTitle = "Aryl", uint32_t aWidth = 1280, uint32_t aHeight = 720, bool aVSync = true, WindowMode aWindowMode = WindowMode::Windowed)
			: title(aTitle), width(aWidth), height(aHeight), vsync(aVSync), windowMode(aWindowMode)
		{
		}

		std::string title;
		uint32_t width;
		uint32_t height;
		bool vsync;
		WindowMode windowMode;
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window(const WindowProperties& aProperties);
		~Window();

		void Shutdown();

		void Invalidate();
		void Release();

		void BeginFrame();
		void Present();

		void Resize(uint32_t aWidth, uint32_t aHeight);
		void SetEventCallback(const EventCallbackFn& callback);
		void SetWindowMode(WindowMode aWindowMode);

		void Maximize() const;
		void Minimize() const;
		void Restore() const;

		const bool IsMaximized() const;

		void ShowCursor(bool aShow) const;

		void SetOpacity(float opacity) const;
		void SetClipboard(const std::string& string);

		const std::pair<float, float> GetPosition() const;

		inline const uint32_t GetWidth() const { return myData.width; }
		inline const uint32_t GetHeight() const { return myData.height; }
		inline const bool IsVSync() const { return myData.vsync; }
		inline const WindowMode GetWindowMode() const { return myData.windowMode; }
		inline GLFWwindow* GetNativeWindow() const { return myWindow; }

		static Scope<Window> Create(const WindowProperties& aProperties = WindowProperties());

	private:
		GLFWwindow* myWindow = nullptr;
		bool myHasBeenInitialized = false;
		bool myIsFullscreen = false;

		struct WindowData
		{
			std::string title;
			uint32_t width;
			uint32_t height;
			bool vsync;
			WindowMode windowMode;

			EventCallbackFn eventCallback;

		} myData;

		glm::uvec2 myStartPosition = { 0u, 0u };
		glm::uvec2 myStartSize = { 0u, 0u };

		WindowProperties myProperties;
	};
}