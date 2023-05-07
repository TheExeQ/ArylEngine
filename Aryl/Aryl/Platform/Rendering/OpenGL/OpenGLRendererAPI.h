#pragma once

#include "Aryl/Renderer/RendererAPI.h"

namespace Aryl
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		OpenGLRendererAPI() = default;
		virtual ~OpenGLRendererAPI() = default;

		virtual void SetClearColor(glm::vec4 color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(Ref<VertexArray> vertexArray) override;
	};
}