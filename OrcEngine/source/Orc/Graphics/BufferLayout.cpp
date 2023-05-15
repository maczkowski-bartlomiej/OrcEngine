#include "orcPch.hpp"
#include "Graphics/BufferLayout.hpp"

#include <glad/glad.h>


namespace orc {

BufferLayout::Element::Element(ShaderDataType type, const std::string& name, bool normalized) 
	: offset(0u), type(type), name(name), size(getShaderDataTypeSize(type)), normalized(normalized)
{
}

uint32 BufferLayout::Element::getComponentCount() const 
{
	switch (type)
	{
		case ShaderDataType::Bool:    return 1u;
		case ShaderDataType::Int:     return 1u;
		case ShaderDataType::Int2:    return 2u;
		case ShaderDataType::Int3:    return 3u;
		case ShaderDataType::Int4:    return 4u;
		case ShaderDataType::Mat3:    return 9u;
		case ShaderDataType::Mat4:    return 16u;
		case ShaderDataType::Float:   return 1u;
		case ShaderDataType::Float2:  return 2u;
		case ShaderDataType::Float3:  return 3u;
		case ShaderDataType::Float4:  return 4u;
		default:					  return 0u;
	}
}

BufferLayout::BufferLayout() 
	: m_stride(0u)
{
}

BufferLayout::BufferLayout(const std::initializer_list<Element>& elements) 
	: m_stride(0u), m_elements(elements)
{
	uint32 offset = 0u;

	for (Element& element : m_elements)
	{
		element.offset = offset;
		offset += element.size;
	}

	m_stride = offset;
}

uint32 BufferLayout::getStride() const 
{
	return m_stride;
}

const std::vector<BufferLayout::Element>& BufferLayout::getElements() const 
{
	return m_elements;
}

uint32 BufferLayout::getShaderDataTypeSize(ShaderDataType type) 
{
	switch (type)
	{
		case ShaderDataType::Bool:    return 1u;
		case ShaderDataType::Mat3:    return 4u * 3u * 3u;
		case ShaderDataType::Mat4:    return 4u * 4u * 4u;
		case ShaderDataType::Int:     return 4u;
		case ShaderDataType::Int2:    return 4u * 2u;
		case ShaderDataType::Int3:    return 4u * 3u;
		case ShaderDataType::Int4:    return 4u * 4u;
		case ShaderDataType::Float:   return 4u;
		case ShaderDataType::Float2:  return 4u * 2u;
		case ShaderDataType::Float3:  return 4u * 3u;
		case ShaderDataType::Float4:  return 4u * 4u;
		default:					  return 0u;
	}
}

uint32 BufferLayout::shaderDataTypeToOpenGLType(ShaderDataType type) 
{
	switch (type)
	{
		case orc::BufferLayout::ShaderDataType::Bool:   return GL_BOOL;
		case orc::BufferLayout::ShaderDataType::Mat3:   return GL_FLOAT;
		case orc::BufferLayout::ShaderDataType::Mat4:   return GL_FLOAT;
		case orc::BufferLayout::ShaderDataType::Int:    return GL_INT;
		case orc::BufferLayout::ShaderDataType::Int2:   return GL_INT;
		case orc::BufferLayout::ShaderDataType::Int3:   return GL_INT;
		case orc::BufferLayout::ShaderDataType::Int4:   return GL_INT;
		case orc::BufferLayout::ShaderDataType::Float:  return GL_FLOAT;
		case orc::BufferLayout::ShaderDataType::Float2: return GL_FLOAT;
		case orc::BufferLayout::ShaderDataType::Float3: return GL_FLOAT;
		case orc::BufferLayout::ShaderDataType::Float4: return GL_FLOAT;
		default:                                          return NULL;
	}
}

}
