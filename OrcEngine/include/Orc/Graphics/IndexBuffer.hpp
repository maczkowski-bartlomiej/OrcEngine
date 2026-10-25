#pragma once

#include <cstdint>
#include <span>

#include "Core/Types.hpp"

namespace orc {

class IndexBuffer
{
public:
	IndexBuffer() = default;
	explicit IndexBuffer(std::span<const uint32_t> indices);

	~IndexBuffer();

	IndexBuffer(IndexBuffer&& other) noexcept;
	IndexBuffer& operator=(IndexBuffer&& other) noexcept;
	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;

	uint32_t getCount() const;
	RendererID getRendererID() const;

private:
	uint32_t m_count = 0;
	RendererID m_rendererID = 0;
};

}
