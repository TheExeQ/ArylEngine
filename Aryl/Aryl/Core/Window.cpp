#include "Window.h"

#include "Aryl/Core/Application.h"

#include "Aryl/Renderer/RendererAPI.h"

#include "Aryl/Events/ApplicationEvent.h"
#include "Aryl/Events/KeyEvent.h"
#include "Aryl/Events/MouseEvent.h"

#include "Aryl/Log/Log.h"

#include "Aryl/Renderer/RenderCommand.h"

#include <glfw/glfw3.h>

namespace Aryl
{
	static void GLFWErrorCallback(int error, const char* description)
	{
		YL_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window::Window(const WindowProperties& aProperties)
	{
		myData.height = aProperties.height;
		myData.width = aProperties.width;
		myData.title = aProperties.title;
		myData.vsync = aProperties.vsync;
		myData.windowMode = aProperties.windowMode;

		myProperties = aProperties;

		Invalidate();

		YL_CORE_INFO("Created window {0} ({1}, {2})", myData.title, myData.width, myData.height);

		myContext = RendererContext::Create(myWindow);
		myContext->Init();

		SetVSync(myProperties.vsync);
	}

	Window::~Window()
	{
		Shutdown();
	}

	void Window::Shutdown()
	{
		Release();
		glfwTerminate();
	}

	void Window::Invalidate()
	{
		if (myWindow)
		{
			Release();
		}

		if (!myHasBeenInitialized)
		{
			if (!glfwInit())
			{
				YL_CORE_ERROR("Failed to initialize GLFW!");
			}
		}

		glfwSetErrorCallback(GLFWErrorCallback);
		glfwWindowHint(GLFW_SAMPLES, 0);
		glfwWindowHint(GLFW_AUTO_ICONIFY, false);

		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			{
				glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
				break;
			}
			default:
			{
				glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
				break;
			}
		}

		myWindow = glfwCreateWindow((int32_t)myData.width, (int32_t)myData.height, myData.title.c_str(), nullptr, nullptr);

		bool isRawMouseMotionSupported = glfwRawMouseMotionSupported();
		if (isRawMouseMotionSupported)
		{
			glfwSetInputMode(myWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		}

		if (myData.windowMode == WindowMode::Fullscreen)
		{
			myIsFullscreen = true;
		}

		if (myData.windowMode != WindowMode::Windowed)
		{
			SetWindowMode(myData.windowMode);
		}

		glfwSetWindowUserPointer(myWindow, &myData);

		glfwSetWindowSizeCallback(myWindow, [](GLFWwindow* window, int32_t width, int32_t height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			int32_t x, y;
			glfwGetWindowPos(window, &x, &y);

			WindowResizeEvent event((uint32_t)x, (uint32_t)y, width, height);
			data.eventCallback(event);
		});

		glfwSetWindowCloseCallback(myWindow, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event{};
			data.eventCallback(event);
		});

		glfwSetKeyCallback(myWindow, [](GLFWwindow* window, int32_t key, int32_t, int32_t action, int32_t)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			if (key == -1)
			{
				return;
			}

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.eventCallback(event);
					break;
				}

				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.eventCallback(event);
					break;
				}

				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.eventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(myWindow, [](GLFWwindow* window, uint32_t key)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			KeyTypedEvent event(key);
			data.eventCallback(event);
		});

		glfwSetMouseButtonCallback(myWindow, [](GLFWwindow* window, int button, int action, int)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.eventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.eventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(myWindow, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.eventCallback(event);
		});

		glfwSetCursorPosCallback(myWindow, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.eventCallback(event);
		});

		glfwSetDropCallback(myWindow, [](GLFWwindow* window, int32_t count, const char** paths)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			WindowDragDropEvent event(count, paths);
			data.eventCallback(event);
		});
	}

	void Window::Release()
	{
		if (myWindow)
		{
			glfwDestroyWindow(myWindow);
			myWindow = nullptr;
		}
	}

	void Window::SetWindowMode(WindowMode aWindowMode)
	{

	}

	void Window::BeginFrame()
	{
		Aryl::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		Aryl::RenderCommand::Clear();
	}

	void Window::Present()
	{
		myContext->SwapBuffers();
		glfwPollEvents();
	}

	void Window::Resize(uint32_t aWidth, uint32_t aHeight)
	{

	}

	void Window::SetEventCallback(const EventCallbackFn& callback)
	{
		myData.eventCallback = callback;
	}

	void Window::Maximize() const
	{
		glfwMaximizeWindow(myWindow);
	}

	void Window::Minimize() const
	{
		glfwIconifyWindow(myWindow);
	}

	void Window::Restore() const
	{
		glfwRestoreWindow(myWindow);
	}

	const bool Window::IsMaximized() const
	{
		return (bool)glfwGetWindowAttrib(myWindow, GLFW_MAXIMIZED);
	}

	void Window::ShowCursor(bool aShow) const
	{
		glfwSetInputMode(myWindow, GLFW_CURSOR, aShow ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}

	void Window::SetOpacity(float opacity) const
	{
		glfwSetWindowOpacity(myWindow, opacity);
	}

	void Window::SetClipboard(const std::string& string)
	{
		glfwSetClipboardString(myWindow, string.c_str());
	}

	void Window::SetVSync(bool aActive)
	{
		myProperties.vsync = aActive;

		if (myProperties.vsync)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
	}

	const std::pair<float, float> Window::GetPosition() const
	{
		int32_t x, y;
		glfwGetWindowPos(myWindow, &x, &y);

		return { (float)x, (float)y };
	}

	Scope<Window> Window::Create(const WindowProperties& aProperties)
	{
		return CreateScope<Window>(aProperties);
	}
}