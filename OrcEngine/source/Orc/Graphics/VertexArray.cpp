#include "OrcPch.hpp"

#include "Graphics/VertexArray.hpp"
#include "Graphics/BufferLayout.hpp"

#include <glad/glad.h>

namespace orc {

VertexArray::VertexArray() 
{
	glCreateVertexArrays(1, &m_rendererID);
}

VertexArray::~VertexArray() 
{
	glDeleteVertexArrays(1, &m_rendererID);
}

void VertexArray::setIndexBuffer(Ref<IndexBuffer> indexBuffer) 
{
	glVertexArrayElementBuffer(m_rendererID, indexBuffer->getRendererID());
	m_indexBuffer = indexBuffer;
}

void VertexArray::addVertexBuffer(Ref<VertexBuffer> vertexBuffer) 
{
	ORC_ASSERT(vertexBuffer->getLayout().getElements().size(), "Vertex Buffer has no layout")

	const BufferLayout& bufferLayout = vertexBuffer->getLayout();
	const std::vector<BufferLayout::Element>& bufferElements = bufferLayout.getElements();

	glVertexArrayVertexBuffer(
		m_rendererID,                  // VAO
		(int)m_vertexBuffers.size(),   // current VBO number linked to VAO
		vertexBuffer->getRendererID(), // VBO
		0,                             // offset of the first element in the VBO
		bufferLayout.getStride()
		);

	for (uint32_t i = 0; i < bufferElements.size(); i++)
	{
		const auto& element = bufferElements[i];

		glEnableVertexArrayAttrib(m_rendererID, i);
		glVertexArrayAttribFormat(
			m_rendererID,
			i,
			element.getComponentCount(),
			BufferLayout::shaderDataTypeToOpenGLType(element.type),
			element.normalized ? GL_TRUE : GL_FALSE,
			element.offset
		);

		glVertexArrayAttribBinding(m_rendererID, i, (int)m_vertexBuffers.size());
	}

	glVertexArrayVertexBuffer(m_rendererID, 0, vertexBuffer->getRendererID(), 0, bufferLayout.getStride());

	m_vertexBuffers.push_back(vertexBuffer);
}

const Ref<IndexBuffer>& VertexArray::getIndexBuffer() const 
{
	return m_indexBuffer;
}

const std::vector<Ref<VertexBuffer>>& VertexArray::getVertexBuffers() const 
{
	return m_vertexBuffers;
}

RendererID VertexArray::getRendererID() const
{
	return m_rendererID;
}

}
