#include "Graphics/Shader.hpp"

#include <fstream>
#include <utility>

#include <glad/glad.h>

#include "Engine/Utility.hpp"

namespace orc {

Shader::Shader(const FilePath& vertexFilePath, const FilePath& fragmentFilePath)
{
	loadFromFile(vertexFilePath, fragmentFilePath);
}

Shader::Shader(Shader&& other) noexcept
	: m_rendererID(std::exchange(other.m_rendererID, 0))
{
}

Shader& Shader::operator=(Shader&& other) noexcept
{
	if (this != &other)
	{
		if (m_rendererID)
		{
			glDeleteProgram(m_rendererID);
		}
		m_rendererID = std::exchange(other.m_rendererID, 0);
	}
	return *this;
}

Shader::~Shader()
{
	if (m_rendererID)
	{
		glDeleteProgram(m_rendererID);
	}
}

void Shader::loadFromFile(const FilePath& vertexFilePath, const FilePath& fragmentFilePath)
{
	std::string vertexShaderSource, fragmentShaderSource;

	readShader(&vertexShaderSource, vertexFilePath);
	readShader(&fragmentShaderSource, fragmentFilePath);
	compile(vertexShaderSource, fragmentShaderSource);
}

void Shader::loadFromString(std::string_view vertexSource, std::string_view fragmentSource)
{
	compile(vertexSource, fragmentSource);
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

void Shader::uploadUniformIntArray(const std::string& name, std::span<const int> values) const
{
	GLint location = glGetUniformLocation(m_rendererID, name.c_str());
	glUniform1iv(location, static_cast<GLsizei>(values.size()), values.data());
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

void Shader::readShader(std::string* shader, const FilePath& filePath)
{
	std::ifstream shaderFile(filePath, std::ios::in | std::ios::binary);
	ORC_FATAL_CHECK(shaderFile.is_open(), "Failed to load shader at path '{}'\n\tReason: {}", filePath.string(), getErrnoMessage(errno));

	std::stringstream buffer;
	buffer << shaderFile.rdbuf();
	*shader = buffer.str();

	shaderFile.close();
}

void Shader::compile(std::string_view vertexSource, std::string_view fragmentSource)
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
		ORC_FATAL("Vertex shader compilation\n{}", errorMessage.data());
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
		ORC_FATAL("Fragment shader compilation\n{}", errorMessage.data());
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
		ORC_FATAL("Shader link failure\n{}", errorMessage.data());
	}

	glDetachShader(m_rendererID, vertexShader);
	glDetachShader(m_rendererID, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

}
