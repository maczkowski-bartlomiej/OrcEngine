#include "OrcPch.hpp"

#include "Graphics/IndexBuffer.hpp"

#include <glad/glad.h>

namespace orc {

IndexBuffer::IndexBuffer() 
	: m_count(0u), m_rendererID(0u)
{
}

IndexBuffer::IndexBuffer(uint32* indices, uint32 count) 
	: m_count(count)
{
	glCreateBuffers(1, &m_rendererID);
	glNamedBufferData(m_rendererID, count * sizeof(uint32), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() 
{
	glDeleteBuffers(1, &m_rendererID);
}

uint32 IndexBuffer::getCount() const 
{
	return m_count;
}

RendererID IndexBuffer::getRendererID() const
{
	return m_rendererID;
}

}
