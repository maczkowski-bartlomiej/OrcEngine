#include "OrcPch.hpp"

#include "Graphics/Texture.hpp"

#include <stb_image.h>
#include <glad/glad.h>

namespace orc {

Texture::Texture()
	: m_rendererID(0)
{
}

Texture::Texture(const FilePath& filePath, bool repeat)
	: m_rendererID(0)
{
	loadFromFile(filePath, repeat);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_rendererID);
}

void Texture::setTextureWrapping(bool repeat)
{
	if (repeat)
	{
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
}

bool Texture::loadFromFile(const FilePath& filePath, bool repeat)
{
	if (m_rendererID)
	{
		glDeleteTextures(1, &m_rendererID);
	}

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

	setTextureWrapping(repeat);

	glTextureSubImage2D(m_rendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, (void*)pixels);

	stbi_image_free((void*)pixels);

	return true;
}

bool Texture::loadFromMemory(const void* data, uint32_t width, uint32_t height, bool repeat)
{
	if (!data)
	{
		ORC_ERROR("Failed to load texture from memory\n\treason: Data is null");
		return false;
	}

	m_size = Vector2f(width, height);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
	glTextureStorage2D(m_rendererID, 1, GL_RGBA8, (GLsizei)width, (GLsizei)height);

	glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	setTextureWrapping(repeat);

	glTextureSubImage2D(m_rendererID, 0, 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, data);

	return true;
}

void Texture::bind(uint32_t slot)
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
