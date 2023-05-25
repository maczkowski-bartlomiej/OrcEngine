#pragma once

#include "Engine/Core.hpp"

namespace orc {

class IndexBuffer
{
public:
	IndexBuffer();
	IndexBuffer(uint32* indices, uint32 count);
	~IndexBuffer();
	
	IndexBuffer(IndexBuffer&&) = delete;
	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer operator=(IndexBuffer&&) = delete;
	IndexBuffer operator=(const IndexBuffer&) = delete;

	uint32 getCount() const;
	RendererID getRendererID() const;

private:
	uint32 m_count;
	RendererID m_rendererID;
};

};
