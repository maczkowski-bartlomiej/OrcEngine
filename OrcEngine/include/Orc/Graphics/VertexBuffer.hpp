#pragma once

#include "System/System.hpp"
#include "Graphics/BufferLayout.hpp"

namespace orc {

class VertexBuffer
{
public:
	VertexBuffer(float* vertices, uint32 size);
	~VertexBuffer();

	VertexBuffer(VertexBuffer&&) = delete;
	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer operator=(VertexBuffer&&) = delete;
	VertexBuffer operator=(const VertexBuffer&) = delete;

	void setLayout(const BufferLayout& bufferLayout);

	RendererID getRendererID() const;
	const BufferLayout& getLayout() const;

private:
	RendererID m_rendererID;
	BufferLayout m_bufferLayout;
};

};
