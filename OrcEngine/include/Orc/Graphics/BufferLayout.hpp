#pragma once

#include "Engine/Core.hpp"

#include <string>
#include <vector>

namespace orc {

class BufferLayout
{
public:
	enum class ShaderDataType
	{
		Invalid = -1,
		Bool,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Float, Float2, Float3, Float4,
	};

	struct Element
	{
		Element(ShaderDataType type, const std::string& name, bool normalized = false);

		uint32_t getComponentCount() const;

		bool normalized;
		std::string name;

		uint32_t size;
		uint32_t offset;
		ShaderDataType type;
	};

	BufferLayout();
	BufferLayout(const std::initializer_list<Element>& elements);

	uint32_t getStride() const;
	const std::vector<Element>& getElements() const;

	static uint32_t getShaderDataTypeSize(ShaderDataType type);
	static uint32_t shaderDataTypeToOpenGLType(ShaderDataType type);

private:
	uint32_t m_stride;
	std::vector<Element> m_elements;
};

}
