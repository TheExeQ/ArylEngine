#include "VertexArray.h"

#include "Aryl/Renderer/Renderer.h"

#include "Platform/Rendering/OpenGL/OpenGLVertexArray.h"

namespace Aryl 
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    YL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexArray>();
		}

		YL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}