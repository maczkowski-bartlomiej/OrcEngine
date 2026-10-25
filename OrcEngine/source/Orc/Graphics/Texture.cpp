#include "Graphics/Texture.hpp"

#include <utility>

#include <glad/glad.h>

#include <stb_image.h>

namespace orc {

Texture::Texture()
{
}

Texture::Texture(const FilePath& filePath)
{
	loadFromFile(filePath);
}

Texture::Texture(Texture&& other) noexcept
	: m_size(other.m_size),
	  m_rendererID(std::exchange(other.m_rendererID, 0))
{
	other.m_size = {0.0f, 0.0f};
}

Texture& Texture::operator=(Texture&& other) noexcept
{
	if (this != &other)
	{
		if (m_rendererID)
		{
			glDeleteTextures(1, &m_rendererID);
		}
		m_size = other.m_size;
		m_rendererID = std::exchange(other.m_rendererID, 0);
		other.m_size = {0.0f, 0.0f};
	}
	return *this;
}

Texture::~Texture()
{
	if (m_rendererID)
	{
		glDeleteTextures(1, &m_rendererID);
	}
}

void Texture::loadFromFile(const FilePath& filePath)
{
	if (m_rendererID)
	{
		glDeleteTextures(1, &m_rendererID);
	}

	int channels = 0, desiredChannels = 0, width = 0, height = 0;
	unsigned char* pixels = stbi_load(filePath.string().c_str(), &width, &height, &channels, desiredChannels);
	ORC_FATAL_CHECK(pixels != nullptr, "Failed to load texture at path '{}'\n\tReason: {}", filePath.string(), stbi_failure_reason());

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

	ORC_FATAL_CHECK(dataFormat != 0 && internalFormat != 0, "Failed to load texture at path '{}'\n\tReason: Texture format not supported", filePath.string());

	m_size = Vector2f(width, height);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
	glTextureStorage2D(m_rendererID, 1, internalFormat, width, height);

	glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTextureSubImage2D(m_rendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, pixels);

	stbi_image_free(pixels);
}

void Texture::loadFromMemory(std::span<const unsigned char> data, uint32_t width, uint32_t height, TextureMode textureMode)
{
	ORC_FATAL_CHECK(!data.empty(), "Failed to load texture from memory\n\tReason: Data is empty");

	if (m_rendererID)
	{
		glDeleteTextures(1, &m_rendererID);
	}

	GLenum openGLTextureMode = 0;
	switch (textureMode)
	{
		case TextureMode::Red:
			openGLTextureMode = GL_RED;
			break;
		case TextureMode::Green:
			openGLTextureMode = GL_GREEN;
			break;
		case TextureMode::Blue:
			openGLTextureMode = GL_BLUE;
			break;
		case TextureMode::Rgb:
			openGLTextureMode = GL_RGB;
			break;
		case TextureMode::Rgba:
			openGLTextureMode = GL_RGBA;
			break;
		default:
			openGLTextureMode = GL_RGBA;
			break;
	}

	m_size = Vector2f(width, height);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
	glTextureStorage2D(m_rendererID, 1, GL_RGBA8, static_cast<GLsizei>(width), static_cast<GLsizei>(height));

	glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTextureSubImage2D(m_rendererID, 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), openGLTextureMode, GL_UNSIGNED_BYTE, data.data());
}

void Texture::bind(uint32_t slot) const
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

RendererID Texture::getRendererID() const
{
	return m_rendererID;
}

}
