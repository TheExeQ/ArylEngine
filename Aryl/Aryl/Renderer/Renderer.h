#pragma once

#include "RenderCommand.h"

namespace Aryl 
{
	class Renderer
	{
	public:

		static void BeginScene();
		static void EndScene();

		static void Submit(Ref<VertexArray> vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}