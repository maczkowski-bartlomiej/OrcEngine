#include "OrcPch.hpp"
#include "Graphics/Shader.hpp"
#include "Engine/Utility.hpp"

#include <glad/glad.h>

namespace orc {

Shader::Shader(const FilePath& vertexFilePath, const FilePath& fragmentFilePath)
{
	loadFromFile(vertexFilePath, fragmentFilePath);
}

Shader::~Shader() 
{
	glDeleteProgram(m_rendererID);
}

bool Shader::loadFromFile(const FilePath& vertexFilePath, const FilePath& fragmentFilePath)
{
	std::string vertexShaderSource, fragmentShaderSource;

	if (!readShader(&vertexShaderSource, vertexFilePath))      return false;
	if (!readShader(&fragmentShaderSource, fragmentFilePath))  return false;
	if (!compile(vertexShaderSource, fragmentShaderSource))    return false;

	return true;
}

bool Shader::loadFromString(std::string_view vertexShaderSource, std::string_view fragmentShaderSource)
{
	if (!compile(vertexShaderSource, fragmentShaderSource))
	{
		return false;
	}

	return true;
}

void Shader::bind() const 
{
	glUseProgram(m_rendererID);
}

void Shader::unbind() const 
{
	glUseProgram(0);
}

void Shader::uploadUniformInt(const std::string& name, int integer) const
{
	GLint location = glGetUniformLocation(m_rendererID, name.c_str());
	glUniform1i(location, integer);
}

void Shader::uploadUniformIntArray(const std::string& name, const int* array, uint32_t size) const
{
	GLint location = glGetUniformLocation(m_rendererID, name.c_str());
	glUniform1iv(location, static_cast<GLsizei>(size), array);
}

void Shader::uploadUniformFloat3(const std::string& name, const Vector3f& float3) const
{
	GLint location = glGetUniformLocation(m_rendererID, name.c_str());
	glUniform3f(location, float3.x, float3.y, float3.z);
}

void Shader::uploadUniformFloat4(const std::string& name, const Vector4f& float4) const
{
	GLint location = glGetUniformLocation(m_rendererID, name.c_str());
	glUniform4f(location, float4.x, float4.y, float4.z, float4.w);
}

void Shader::uploadUniformMatrix4(const std::string& name, const Matrix4& matrix) const
{
	GLint location = glGetUniformLocation(m_rendererID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::uploadUniformMatrix3(const std::string& name, const Matrix& matrix) const
{
	GLint location = glGetUniformLocation(m_rendererID, name.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

bool Shader::readShader(std::string* shader, const FilePath& filePath)
{
	bool success = true;

	std::ifstream shaderFile(filePath, std::ios::in | std::ios::binary);
	if (!shaderFile.is_open())
	{
		ORC_ERROR("Failed to load shader at path '{}'\n\tReason: {}", filePath.string(), getErrnoMessage(errno));
		success = false;
	}
	else if (!shaderFile.good())
	{
		ORC_ERROR("Failed to load shader at path '{}'\n\tReason: {}", filePath.string(), getErrnoMessage(errno));
		success = false;
	}
	else
	{
		std::stringstream buffer;
		buffer << shaderFile.rdbuf();
		*shader = buffer.str();
	}

	shaderFile.close();
	return success;
}

bool Shader::compile(std::string_view vertexSource, std::string_view fragmentSource)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	
	const char* vSource = vertexSource.data();
	GLint vLength = static_cast<GLint>(vertexSource.size());
	glShaderSource(vertexShader, 1, &vSource, &vLength);
	glCompileShader(vertexShader);
	GLint status = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint errorLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &errorLength);
		std::vector<char> errorMessage(static_cast<uint32_t>(errorLength));
		glGetShaderInfoLog(vertexShader, errorLength, &errorLength, errorMessage.data());
		glDeleteShader(vertexShader);
		ORC_ERROR("Vertex shader compilation\n{}", errorMessage.data());

		return false;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fSource = fragmentSource.data();
	GLint fLength = static_cast<GLint>(fragmentSource.size());
	glShaderSource(fragmentShader, 1, &fSource, &fLength);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint errorLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &errorLength);
		std::vector<char> errorMessage(static_cast<uint32_t>(errorLength));
		glGetShaderInfoLog(fragmentShader, errorLength, &errorLength, errorMessage.data());
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		ORC_ERROR("Fragment shader compilation\n{}", errorMessage.data());

		return false;
	}

	glDeleteProgram(m_rendererID);
	m_rendererID = glCreateProgram();
	glAttachShader(m_rendererID, vertexShader);
	glAttachShader(m_rendererID, fragmentShader);
	glLinkProgram(m_rendererID);
	glGetProgramiv(m_rendererID, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_rendererID, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> errorMessage(static_cast<uint32_t>(maxLength));
		glGetProgramInfoLog(m_rendererID, maxLength, &maxLength, errorMessage.data());
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(m_rendererID);
		ORC_ERROR("Shader link failure\n{}", errorMessage.data());

		return false;
	}

	glDetachShader(m_rendererID, vertexShader);
	glDetachShader(m_rendererID, fragmentShader);

	return true;
}

}
