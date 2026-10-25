#pragma once

#include <cstdint>
#include <span>

#include "Core/Types.hpp"
#include "Graphics/BufferLayout.hpp"

namespace orc {

class VertexBuffer
{
public:
	explicit VertexBuffer(uint32_t size);

	template <typename T>
	explicit VertexBuffer(std::span<T> vertices)
	{
		init(vertices.data(), static_cast<uint32_t>(vertices.size_bytes()), false);
	}

	~VertexBuffer();

	VertexBuffer(VertexBuffer&& other) noexcept;
	VertexBuffer& operator=(VertexBuffer&& other) noexcept;
	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;

	template <typename T>
	void setData(std::span<T> data)
	{
		setDataInternal(data.data(), static_cast<uint32_t>(data.size_bytes()));
	}

	void setLayout(const BufferLayout& bufferLayout);

	RendererID getRendererID() const;
	const BufferLayout& getLayout() const;

private:
	void init(const void* data, uint32_t size, bool dynamic);
	void setDataInternal(const void* data, uint32_t size);

	RendererID m_rendererID = 0;
	BufferLayout m_bufferLayout;
};

}
