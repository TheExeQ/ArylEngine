#include "OpenGLContext.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace Aryl
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: myWindowHandle(windowHandle)
	{
		YL_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(myWindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		YL_CORE_ASSERT(status, "Failed to initialize Glad!");

		YL_CORE_INFO("OpenGL Info:");
		YL_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		YL_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		YL_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(myWindowHandle);
	}
}