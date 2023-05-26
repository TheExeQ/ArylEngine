#include "Texture.h"

#include "Aryl/Renderer/Renderer.h"

#include "Platform/Renderer/OpenGL/OpenGLTexture.h"

namespace Aryl
{
	Ref<Texture2D> Texture2D::Create(const std::filesystem::path& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    YL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(filepath);
		}

		YL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}