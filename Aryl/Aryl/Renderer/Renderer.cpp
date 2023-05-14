#include "Renderer.h"

namespace Aryl
{

	void Renderer::BeginScene(const Ref<Camera>& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera->GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(Ref<Shader> shader, Ref<VertexArray> vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->UploadUniformMat4("u_ModelTransform", transform);

		vertexArray->Bind();

		RenderCommand::DrawIndexed(vertexArray);
	}
}
