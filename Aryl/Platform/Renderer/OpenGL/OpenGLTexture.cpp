#include "OpenGLTexture.h"

#include "Aryl/Renderer/Renderer.h"

#include <stb/stb_image.h>
#include <glad/glad.h>

namespace Aryl
{
	OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path& filepath)
		: myFilepath(filepath)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(filepath.string().c_str(), &width, &height, &channels, 0);
		YL_CORE_ASSERT(data, "Failed to load image!");

		myWidth = width;
		myHeight = height;

		GLenum internalFormat = 0, dataFormat = 0;

		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		YL_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &myRendererID);
		glTextureStorage2D(myRendererID, 1, internalFormat, myWidth, myHeight);

		glTextureParameteri(myRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(myRendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureSubImage2D(myRendererID, 0, 0, 0, myWidth, myHeight, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &myRendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, myRendererID);
	}
}