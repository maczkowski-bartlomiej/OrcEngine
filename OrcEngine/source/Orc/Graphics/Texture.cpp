#include "OrcPch.hpp"

#include "Graphics/Texture.hpp"

#include <stb_image.h>
#include <glad/glad.h>

namespace orc {

Texture::Texture()
	: m_rendererID(0u)
{
}

Texture::Texture(const FilePath& filePath)
	: m_rendererID(0u)
{
	loadFromFile(filePath);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_rendererID);
}

bool Texture::loadFromFile(const FilePath& filePath)
{
	int channels = 0, desiredChannels = 0, width = 0, height = 0;
	unsigned char* pixels = stbi_load(filePath.string().c_str(), &width, &height, &channels, desiredChannels);

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

	m_size = Vector2f(width, height);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
	glTextureStorage2D(m_rendererID, 1, internalFormat, width, height);

	glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureSubImage2D(m_rendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, (void*)pixels);

	stbi_image_free((void*)pixels);

	return true;
}

void Texture::bind(uint32 slot)
{
	glBindTextureUnit(slot, m_rendererID);
}

float Texture::getWidth() const
{
	return m_size.x;
}

float Texture::getHeight() const
{
	return m_size.y;
}

Vector2f Texture::getSize() const
{
	return m_size;
}

}
