#include "RendererAPI.h"

#include "Aryl/Renderer/Renderer.h"
#include "Platform/Rendering/OpenGL/OpenGLRendererAPI.h"

namespace Aryl 
{
	Ref<RendererAPI> RendererAPI::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    YL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLRendererAPI>();
		}

		YL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}