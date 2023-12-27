#include "OrcPch.hpp"

#include "Graphics/IndexBuffer.hpp"

#include <glad/glad.h>

namespace orc {

IndexBuffer::IndexBuffer() 
	: m_count(0), m_rendererID(0)
{
}

IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) 
	: m_count(count)
{
	glCreateBuffers(1, &m_rendererID);
	glNamedBufferData(m_rendererID, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() 
{
	glDeleteBuffers(1, &m_rendererID);
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
