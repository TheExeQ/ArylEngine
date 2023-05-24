#pragma once

#include "Aryl/Core/Base.h"
#include "Aryl/Renderer/Texture.h"

#include <filesystem>

namespace Aryl
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::filesystem::path& filepath);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return myWidth; };
		virtual uint32_t GetHeight() const override { return myHeight; };

		virtual void Bind(uint32_t slot = 0) const override;

	private:
		std::filesystem::path myFilepath;
		uint32_t myWidth, myHeight;
		uint32_t myRendererID;
	};
}