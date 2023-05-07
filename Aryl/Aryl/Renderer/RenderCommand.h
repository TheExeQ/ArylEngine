#pragma once

#include "Aryl/Core/Base.h"

#include "RendererAPI.h"

namespace Aryl
{
	class RenderCommand
	{
	public:
		inline static void SetClearColor(glm::vec4 color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(Ref<VertexArray> vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

	private:
		inline static Ref<RendererAPI> s_RendererAPI = RendererAPI::Create();
	};
}