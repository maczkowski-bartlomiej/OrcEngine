#pragma once

#include "Engine/Core.hpp"

#include <cstdint>
#include <span>

namespace orc {

class IndexBuffer
{
public:
	IndexBuffer() = default;
	IndexBuffer(const uint32_t* indices, uint32_t count);

	template<typename T>
	explicit IndexBuffer(std::span<T> indices)
		: IndexBuffer(indices.data(), static_cast<uint32_t>(indices.size()))
	{
	}

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

}
