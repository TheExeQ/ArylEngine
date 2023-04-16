#include "RendererContext.h"
#include "RendererAPI.h"

#include "Aryl/Platform/Rendering/OpenGL/OpenGLContext.h"

namespace Aryl
{
	Ref<RendererContext> RendererContext::Create(GLFWwindow* windowHandle)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:    return nullptr;
			case RendererAPIType::OpenGL:  return CreateRef<OpenGLContext>(windowHandle);
			case RendererAPIType::Vulkan:  YL_CORE_ASSERT(false, "Vulkan is currently not supported!") break;
		}

		YL_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}