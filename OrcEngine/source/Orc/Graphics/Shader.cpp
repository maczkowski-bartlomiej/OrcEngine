#include "OrcPch.hpp"

#include "Graphics/Shader.hpp"

#include "Utility.hpp"

#include <glad/glad.h>

namespace orc {

Shader::Shader()
	: m_rendererID(0)
{
}

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

bool Shader::loadFromString(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
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
	glUseProgram(NULL);
}

void Shader::uploadUniformInt(const std::string& name, int integer) const
{
	GLint location = glGetUniformLocation(m_rendererID, name.c_str());
	glUniform1i(location, integer);
}


void Shader::uploadUniformIntArray(const std::string& name, int* array, uint32_t size) const
{
	GLint location = glGetUniformLocation(m_rendererID, name.c_str());
	glUniform1iv(location, size, array);
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
		ORC_ERROR("Failed to load shader at path '{}'\n\treason: {}", filePath.string(), utility::getErrnoMessage(errno));
		success = false;
	}
	else if (!shaderFile.good())
	{
		ORC_ERROR("Failed to load shader at path '{}'\n\treason: {}", filePath.string(), utility::getErrnoMessage(errno));
		success = false;
	}
	else
	{
		//Copy entire file content to buffer, then call buffer.str() to retrive it as string
		std::stringstream buffer;
		buffer << shaderFile.rdbuf();
		*shader = buffer.str();
	}

	shaderFile.close();
	return success;
}

bool Shader::compile(const std::string& vertexSource, const std::string& fragmentSource)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	
	const char* source = vertexSource.c_str();
	glShaderSource(vertexShader, 1, &source, 0);
	glCompileShader(vertexShader);
	int status = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		int errorLenght = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &errorLenght);
		std::vector<char> errorMessage(errorLenght);
		glGetShaderInfoLog(vertexShader, errorLenght, &errorLenght, &errorMessage[0]);
		glDeleteShader(vertexShader);
		ORC_ERROR("Vertex shader compilation\n{}", errorMessage.data());

		return false;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	source = fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &source, 0);
	glCompileShader(fragmentShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		int errorLenght = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &errorLenght);
		std::vector<char> errorMessage(errorLenght);
		glGetShaderInfoLog(vertexShader, errorLenght, &errorLenght, &errorMessage[0]);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		ORC_ERROR("Vertex shader compilation\n{}", errorMessage.data());

		return false;
	}

	m_rendererID = glCreateProgram();
	glAttachShader(m_rendererID, vertexShader);
	glAttachShader(m_rendererID, fragmentShader);
	glLinkProgram(m_rendererID);
	glGetProgramiv(m_rendererID, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint maxLenght = 0;
		glGetProgramiv(m_rendererID, GL_INFO_LOG_LENGTH, &maxLenght);
		std::vector<char> errorMessage(maxLenght);
		glGetProgramInfoLog(m_rendererID, maxLenght, &maxLenght, &errorMessage[0]);
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
