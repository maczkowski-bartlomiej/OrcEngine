#pragma once

#include "Engine/Core.hpp"
#include "Graphics/BufferLayout.hpp"

namespace orc {

class VertexBuffer
{
public:
	VertexBuffer(void* vertices, uint32_t size);
	~VertexBuffer();

	VertexBuffer(VertexBuffer&&) = delete;
	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer operator=(VertexBuffer&&) = delete;
	VertexBuffer operator=(const VertexBuffer&) = delete;

	void setData(void* data, uint32_t size);
	void setLayout(const BufferLayout& bufferLayout);

	RendererID getRendererID() const;
	const BufferLayout& getLayout() const;

private:
	RendererID m_rendererID = 0;
	BufferLayout m_bufferLayout;
};

};
