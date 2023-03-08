#include "OpenGLContext.h"

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
	}
}