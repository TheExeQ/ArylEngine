#include "Shader.h"

#include "Aryl/Renderer/Renderer.h"

#include "Platform/Renderer/OpenGL/OpenGLShader.h"

namespace Aryl 
{
	Ref<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    YL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLShader>(vertexSrc, fragmentSrc);
		}

		YL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}