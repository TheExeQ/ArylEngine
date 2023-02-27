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

		static Ref<RendererContext> Create();
	};
}