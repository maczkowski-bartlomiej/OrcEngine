#pragma once

#include <string>
#include <vector>

#include "System/System.hpp"

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

		uint32 getComponentCount() const;

		bool normalized;
		std::string name;

		uint32 size;
		uint32 offset;
		ShaderDataType type;
	};

	BufferLayout();
	BufferLayout(const std::initializer_list<Element>& elements);

	uint32 getStride() const;
	const std::vector<Element>& getElements() const;

	static uint32 getShaderDataTypeSize(ShaderDataType type);
	static uint32 shaderDataTypeToOpenGLType(ShaderDataType type);

private:
	uint32 m_stride;
	std::vector<Element> m_elements;
};

}
