#pragma once

#include "Aryl/Core/Base.h"

#include "VertexArray.h"

namespace Aryl 
{
	enum class PrimitiveType
	{
		None = 0, Triangles, Lines
	};

	class RendererAPI
	{
	public:
		enum class API
		{
			None,
			OpenGL,
			Vulkan
		};

		virtual void SetClearColor(glm::vec4 color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(Ref<VertexArray> vertexArray) = 0;

		static API GetAPI() { return s_CurrentRendererAPI; }
		static void SetAPI(API api) { s_CurrentRendererAPI = api; }

		static Ref<RendererAPI> Create();

	private:
		inline static API s_CurrentRendererAPI = API::OpenGL;
	};
}