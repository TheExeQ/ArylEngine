#pragma once

#include "Shader.h"
#include "RendererAPI.h"

namespace Aryl 
{
	class Renderer
	{
	public:
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	};
}