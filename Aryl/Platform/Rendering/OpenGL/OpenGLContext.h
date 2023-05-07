#pragma once

#include "Aryl/Renderer/RendererContext.h"

namespace Aryl
{
	class OpenGLContext : public RendererContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		void Init() override;
		void SwapBuffers() override;

	private:
		GLFWwindow* myWindowHandle;
	};
}