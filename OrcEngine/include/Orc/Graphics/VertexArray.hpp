#pragma once

#include "Engine/Core.hpp"

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

	void setIndexBuffer(Ref<IndexBuffer> indexBuffer);
	void addVertexBuffer(Ref<VertexBuffer> vertexBuffer);

	const Ref<IndexBuffer>& getIndexBuffer() const;
	const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const;

	RendererID getRendererID() const;

private:
	RendererID m_rendererID;
	Ref<IndexBuffer> m_indexBuffer;
	std::vector<Ref<VertexBuffer>> m_vertexBuffers;
};

};
