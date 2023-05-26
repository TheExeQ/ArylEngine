#include "RendererContext.h"
#include "RendererAPI.h"

#include "Platform/Renderer/OpenGL/OpenGLContext.h"

namespace Aryl
{
	Ref<RendererContext> RendererContext::Create(GLFWwindow* windowHandle)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLContext>(windowHandle);
			case RendererAPI::API::Vulkan:  YL_CORE_ASSERT(false, "Vulkan is currently not supported!") break;
		}

		YL_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}