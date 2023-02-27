#include "RendererContext.h"
#include "RendererAPI.h"

namespace Aryl
{
	Ref<RendererContext> RendererContext::Create()
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:    return nullptr;
			case RendererAPIType::OpenGL:  return nullptr;
			case RendererAPIType::Vulkan:  YL_CORE_ASSERT(false, "Vulkan is currently not supported!") break;
		}

		YL_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}