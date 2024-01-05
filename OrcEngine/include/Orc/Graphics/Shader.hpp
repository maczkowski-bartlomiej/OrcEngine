#pragma once

#include "Engine/Core.hpp"

#include <string>

namespace orc {

class Shader
{
public:
	Shader();
	Shader(const FilePath& vertexFilePath, const FilePath& fragmentFilePath);
	~Shader();

	Shader(Shader&&) = delete;
	Shader(const Shader&) = delete;
	Shader operator=(Shader&&) = delete;
	Shader operator=(const Shader&) = delete;

	bool loadFromFile(const FilePath& vertexFilePath, const FilePath& fragmentFilePath);
	bool loadFromString(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

	void bind() const;
	void unbind() const;

	void uploadUniformInt(const std::string& name, int integer) const;
	void uploadUniformIntArray(const std::string& name, int* array, uint32_t size) const;

	void uploadUniformFloat3(const std::string& name, const Vector3f& float3) const;
	void uploadUniformFloat4(const std::string& name, const Vector4f& float4) const;
	void uploadUniformMatrix4(const std::string& name, const Matrix4& matrix) const;

	void uploadUniformMatrix3(const std::string& name, const Matrix& matrix) const;

private:
	bool readShader(std::string* shader, const FilePath& filePath);
	bool compile(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

	RendererID m_rendererID;
};

}
