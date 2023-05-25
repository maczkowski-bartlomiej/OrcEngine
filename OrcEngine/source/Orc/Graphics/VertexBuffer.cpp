#include "OrcPch.hpp"

#include "Graphics/VertexBuffer.hpp"

#include <glad/glad.h>

namespace orc {

VertexBuffer::VertexBuffer(float* vertices, uint32 size) 
{
	glCreateBuffers(1, &m_rendererID);
	glNamedBufferData(m_rendererID, size, vertices, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() 
{
	glDeleteBuffers(1, &m_rendererID);
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
