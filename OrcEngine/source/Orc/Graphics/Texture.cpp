#include "OrcPch.hpp"

#include "Graphics/Texture.hpp"

#include <stb_image.h>
#include <glad/glad.h>

namespace orc {

Texture::Texture()
	: m_width(0), m_height(0), m_rendererID(0u)
{
}

Texture::Texture(const FilePath& filePath)
	: m_width(0), m_height(0), m_rendererID(0u)
{
	loadFromFile(filePath);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_rendererID);
}

bool Texture::loadFromFile(const FilePath& filePath)
{
	int channels = 0, desiredChannels = 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(filePath.string().c_str(), &m_width, &m_height, &channels, desiredChannels);

	if (!pixels)
	{
		ORC_ERROR("Failed to load texture at path '{}'\n\treason: {}", filePath.string(), stbi_failure_reason());
		return false;
	}
	
	GLenum dataFormat = 0, internalFormat = 0;
	if (channels == 4)
	{
		dataFormat = GL_RGBA;
		internalFormat = GL_RGBA8;
	}
	else if (channels == 3)
	{
		dataFormat = GL_RGB;
		internalFormat = GL_RGB8;
	}

	if (!dataFormat && !internalFormat)
	{
		ORC_ERROR("Failed to load texture at path '{}'\n\treason: Texture format not supported", filePath.string());
		return false;
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
	glTextureStorage2D(m_rendererID, 1, internalFormat, m_width, m_height);

	glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, dataFormat, GL_UNSIGNED_BYTE, (void*)pixels);

	stbi_image_free((void*)pixels);

	return true;
}

void Texture::bind(uint32 slot)
{
	glBindTextureUnit(slot, m_rendererID);
}

int32 Texture::getWidth() const
{
	return m_width;
}

int32 Texture::getHeight() const
{
	return m_height;
}

}
