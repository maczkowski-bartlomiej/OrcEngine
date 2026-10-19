#include "OrcPch.hpp"
#include "Graphics/IndexBuffer.hpp"

#include <glad/glad.h>
#include <utility>

namespace orc {

IndexBuffer::IndexBuffer(const uint32_t* indices, uint32_t count)
	: m_count(count)
{
	glCreateBuffers(1, &m_rendererID);
	glNamedBufferData(m_rendererID, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
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
