#include "Graphics/VertexArray.hpp"

#include <glad/glad.h>

#include "Graphics/BufferLayout.hpp"

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
	m_indexBuffer = std::move(indexBuffer);
}

void VertexArray::addVertexBuffer(Ref<VertexBuffer> vertexBuffer)
{
	ORC_FATAL_CHECK(!vertexBuffer->getLayout().getElements().empty(), "Vertex Buffer has no layout");

	const BufferLayout& bufferLayout = vertexBuffer->getLayout();
	const std::vector<BufferLayout::Element>& bufferElements = bufferLayout.getElements();
	glVertexArrayVertexBuffer(
		m_rendererID,								   // VAO
		static_cast<uint32_t>(m_vertexBuffers.size()), // current VBO number linked to VAO
		vertexBuffer->getRendererID(),				   // VBO
		0,											   // offset of the first element in the VBO
		static_cast<GLsizei>(bufferLayout.getStride()));

	for (uint32_t i = 0; i < bufferElements.size(); i++)
	{
		const auto& element = bufferElements[i];

		glEnableVertexArrayAttrib(m_rendererID, i);
		glVertexArrayAttribFormat(
			m_rendererID,
			i,
			static_cast<GLint>(element.getComponentCount()),
			BufferLayout::shaderDataTypeToOpenGLType(element.type),
			element.normalized ? GL_TRUE : GL_FALSE,
			element.offset);

		glVertexArrayAttribBinding(m_rendererID, i, static_cast<uint32_t>(m_vertexBuffers.size()));
	}

	m_vertexBuffers.push_back(std::move(vertexBuffer));
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
