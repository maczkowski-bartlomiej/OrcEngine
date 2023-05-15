#include "orcPch.hpp"
#include "Graphics/Shader.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace orc {

Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource) 
{
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
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

		ORC_CORE_LOG_ERROR("{}", errorMessage.data());
		ORC_CORE_ASSERT(false, "Vertex shader compilation failure");
	}

	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
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

		ORC_CORE_LOG_ERROR("{}", errorMessage.data());
		ORC_CORE_ASSERT(false, "Fragment shader compilation failure");
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
		ORC_CORE_LOG_ERROR("{}", errorMessage.data());
		ORC_CORE_ASSERT(false, "Shader link failure");
	}

	glDetachShader(m_rendererID, vertexShader);
	glDetachShader(m_rendererID, fragmentShader);
}

Shader::~Shader() 
{
	glDeleteProgram(m_rendererID);
}

void Shader::bind() const 
{
	glUseProgram(m_rendererID);
}

void Shader::unbind() const 
{
	glUseProgram(NULL);
}

void Shader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix) const 
{
	GLint location = glGetUniformLocation(m_rendererID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

}
