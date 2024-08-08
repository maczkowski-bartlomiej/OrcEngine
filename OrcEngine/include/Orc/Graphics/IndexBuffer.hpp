#pragma once

#include "Engine/Core.hpp"

namespace orc {

class IndexBuffer
{
public:
	IndexBuffer() = default;
	IndexBuffer(uint32_t* indices, uint32_t count);
	~IndexBuffer();
	
	IndexBuffer(IndexBuffer&&) = delete;
	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer operator=(IndexBuffer&&) = delete;
	IndexBuffer operator=(const IndexBuffer&) = delete;

	uint32_t getCount() const;
	RendererID getRendererID() const;

private:
	uint32_t m_count = 0;
	RendererID m_rendererID = 0;
};

};
