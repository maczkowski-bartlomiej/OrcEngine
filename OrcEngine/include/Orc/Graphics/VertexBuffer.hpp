#pragma once

#include "Engine/Core.hpp"
#include "Graphics/BufferLayout.hpp"

#include <cstdint>
#include <span>

namespace orc {

class VertexBuffer
{
public:
	VertexBuffer(const void* vertices, uint32_t size);

	template<typename T>
	explicit VertexBuffer(std::span<T> vertices)
		: VertexBuffer(vertices.data(), static_cast<uint32_t>(vertices.size_bytes()))
	{
	}

	~VertexBuffer();

	VertexBuffer(VertexBuffer&&) = delete;
	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer operator=(VertexBuffer&&) = delete;
	VertexBuffer operator=(const VertexBuffer&) = delete;

	void setData(const void* data, uint32_t size);

	template<typename T>
	void setData(std::span<T> data)
	{
		setData(data.data(), static_cast<uint32_t>(data.size_bytes()));
	}

	void setLayout(const BufferLayout& bufferLayout);

	RendererID getRendererID() const;
	const BufferLayout& getLayout() const;

private:
	RendererID m_rendererID = 0;
	BufferLayout m_bufferLayout;
};

}
