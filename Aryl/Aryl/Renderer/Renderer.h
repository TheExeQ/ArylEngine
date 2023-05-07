#pragma once

#include "RenderCommand.h"

#include "Shader.h"
#include "Camera/Camera.h"

namespace Aryl 
{
	class Renderer
	{
	public:
		static void BeginScene(const Ref<Camera>& camera);
		static void EndScene();

		static void Submit(Ref<Shader> shader, Ref<VertexArray> vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		inline static Ref<SceneData> s_SceneData = CreateRef<SceneData>();
	};
}