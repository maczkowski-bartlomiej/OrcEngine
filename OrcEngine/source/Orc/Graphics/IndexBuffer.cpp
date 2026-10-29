#include "Graphics/IndexBuffer.hpp"

#include <utility>

#include <glad/glad.h>

namespace orc {

IndexBuffer::IndexBuffer(std::span<const uint32_t> indices)
	: m_count(static_cast<uint32_t>(indices.size()))
{
	glCreateBuffers(1, &m_rendererID);
	glNamedBufferData(m_rendererID, indices.size_bytes(), indices.data(), GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
	: m_count(std::exchange(other.m_count, 0)),
	  m_rendererID(std::exchange(other.m_rendererID, 0))
{
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
{
	if (this != &other)
	{
		if (m_rendererID)
		{
			glDeleteBuffers(1, &m_rendererID);
		}
		m_count = std::exchange(other.m_count, 0);
		m_rendererID = std::exchange(other.m_rendererID, 0);
	}
	return *this;
}

IndexBuffer::~IndexBuffer()
{
	if (m_rendererID)
	{
		glDeleteBuffers(1, &m_rendererID);
	}
}

uint32_t IndexBuffer::getCount() const
{
	return m_count;
}

RendererID IndexBuffer::getRendererID() const
{
	return m_rendererID;
}

}
