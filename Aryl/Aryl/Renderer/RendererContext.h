#pragma once

#include "Aryl/Core/Base.h"

struct GLFWwindow;

namespace Aryl
{
	class RendererContext
	{
	public:
		RendererContext() = default;
		virtual ~RendererContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static RendererContext* Create(GLFWwindow* windowHandle);
	};
}