#pragma once

#include "System/System.hpp"

#include <string>
#include <glm/glm.hpp>

namespace orc {

class Shader
{
public:
	Shader(const std::string& vertexSource, const std::string& fragmentSource);
	~Shader();

	Shader(Shader&&) = delete;
	Shader(const Shader&) = delete;
	Shader operator=(Shader&&) = delete;
	Shader operator=(const Shader&) = delete;

	void bind() const;
	void unbind() const;

	void uploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;

private:
	RendererID m_rendererID;
};

}
