#pragma once

#include "System/System.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Graphics/VertexBuffer.hpp"

namespace orc {

class VertexArray
{
public:
	VertexArray();
 	~VertexArray();

	VertexArray(VertexArray&&) = delete;
	VertexArray(const VertexArray&) = delete;
	VertexArray operator=(VertexArray&&) = delete;
	VertexArray operator=(const VertexArray&) = delete;

	void setIndexBuffer(Reference<IndexBuffer> indexBuffer);
	void addVertexBuffer(Reference<VertexBuffer> vertexBuffer);

	const Reference<IndexBuffer>& getIndexBuffer() const;
	const std::vector<Reference<VertexBuffer>>& getVertexBuffers() const;

	RendererID getRendererID() const;

private:
	RendererID m_rendererID;
	Reference<IndexBuffer> m_indexBuffer;
	std::vector<Reference<VertexBuffer>> m_vertexBuffers;
};

};
