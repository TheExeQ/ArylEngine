#include "Shader.h"

#include "Aryl/Renderer/Renderer.h"
#include "Aryl/Platform/Rendering/OpenGL/OpenGLShader.h"

#include <glad/glad.h>

namespace Aryl 
{
	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    YL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		YL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}