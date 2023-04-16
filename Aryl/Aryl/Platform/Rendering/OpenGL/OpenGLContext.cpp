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
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(myWindowHandle);
	}
}