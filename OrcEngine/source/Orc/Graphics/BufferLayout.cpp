#include "OrcPch.hpp"
#include "Graphics/BufferLayout.hpp"

#include <glad/glad.h>

namespace orc {

BufferLayout::Element::Element(ShaderDataType type, const std::string& name, bool normalized) 
	: normalized(normalized), name(name), size(getShaderDataTypeSize(type)), offset(0), type(type)
{
}

uint32_t BufferLayout::Element::getComponentCount() const 
{
	switch (type)
	{
		case ShaderDataType::Bool:		return 1;
		case ShaderDataType::Int:		return 1;
		case ShaderDataType::Int2:		return 2;
		case ShaderDataType::Int3:		return 3;
		case ShaderDataType::Int4:		return 4;
		case ShaderDataType::Matrix3:	return 9;
		case ShaderDataType::Matrix4:	return 16;
		case ShaderDataType::Float:		return 1;
		case ShaderDataType::Float2:	return 2;
		case ShaderDataType::Float3:	return 3;
		case ShaderDataType::Float4:	return 4;

		case ShaderDataType::Invalid:
		default:						return 0u;
	}
}

BufferLayout::BufferLayout() 
	: m_stride(0)
{
}

BufferLayout::BufferLayout(const std::initializer_list<Element>& elements) 
	: m_stride(0), m_elements(elements)
{
	uint32_t offset = 0;

	for (Element& element : m_elements)
	{
		element.offset = offset;
		offset += element.size;
	}

	m_stride = offset;
}

uint32_t BufferLayout::getStride() const 
{
	return m_stride;
}

const std::vector<BufferLayout::Element>& BufferLayout::getElements() const 
{
	return m_elements;
}

uint32_t BufferLayout::getShaderDataTypeSize(ShaderDataType type) 
{
	switch (type)
	{
		case ShaderDataType::Bool:		return 1;
		case ShaderDataType::Matrix3:	return 4 * 3 * 3;
		case ShaderDataType::Matrix4:	return 4 * 4 * 4;
		case ShaderDataType::Int:		return 4;
		case ShaderDataType::Int2:		return 4 * 2;
		case ShaderDataType::Int3:		return 4 * 3;
		case ShaderDataType::Int4:		return 4 * 4;
		case ShaderDataType::Float:		return 4;
		case ShaderDataType::Float2:	return 4 * 2;
		case ShaderDataType::Float3:	return 4 * 3;
		case ShaderDataType::Float4:	return 4 * 4;

		case ShaderDataType::Invalid:
		default:					     return 0u;
	}
}

uint32_t BufferLayout::shaderDataTypeToOpenGLType(ShaderDataType type) 
{
	switch (type)
	{
		case orc::BufferLayout::ShaderDataType::Bool:		return GL_BOOL;
		case orc::BufferLayout::ShaderDataType::Matrix3:	return GL_FLOAT;
		case orc::BufferLayout::ShaderDataType::Matrix4:	return GL_FLOAT;
		case orc::BufferLayout::ShaderDataType::Int:		return GL_INT;
		case orc::BufferLayout::ShaderDataType::Int2:		return GL_INT;
		case orc::BufferLayout::ShaderDataType::Int3:		return GL_INT;
		case orc::BufferLayout::ShaderDataType::Int4:		return GL_INT;
		case orc::BufferLayout::ShaderDataType::Float:		return GL_FLOAT;
		case orc::BufferLayout::ShaderDataType::Float2:		return GL_FLOAT;
		case orc::BufferLayout::ShaderDataType::Float3:		return GL_FLOAT;
		case orc::BufferLayout::ShaderDataType::Float4:		return GL_FLOAT;

		case ShaderDataType::Invalid:
		default:											return 0u;
	}
}

}
