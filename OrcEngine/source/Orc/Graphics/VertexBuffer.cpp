#include "Graphics/VertexBuffer.hpp"

#include <utility>

#include <glad/glad.h>

namespace orc {

VertexBuffer::VertexBuffer(uint32_t size)
{
	init(nullptr, size, true);
}

void VertexBuffer::init(const void* data, uint32_t size, bool dynamic)
{
	glCreateBuffers(1, &m_rendererID);
	glNamedBufferData(m_rendererID, size, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
	: m_rendererID(std::exchange(other.m_rendererID, 0)),
	  m_bufferLayout(std::move(other.m_bufferLayout))
{
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
	if (this != &other)
	{
		if (m_rendererID)
		{
			glDeleteBuffers(1, &m_rendererID);
		}
		m_rendererID = std::exchange(other.m_rendererID, 0);
		m_bufferLayout = std::move(other.m_bufferLayout);
	}
	return *this;
}

VertexBuffer::~VertexBuffer()
{
	if (m_rendererID)
	{
		glDeleteBuffers(1, &m_rendererID);
	}
}

void VertexBuffer::setDataInternal(const void* data, uint32_t size)
{
	glNamedBufferSubData(m_rendererID, 0, size, data);
}

const BufferLayout& VertexBuffer::getLayout() const
{
	return m_bufferLayout;
}

RendererID VertexBuffer::getRendererID() const
{
	return m_rendererID;
}

void VertexBuffer::setLayout(const BufferLayout& bufferLayout)
{
	m_bufferLayout = bufferLayout;
}

}
