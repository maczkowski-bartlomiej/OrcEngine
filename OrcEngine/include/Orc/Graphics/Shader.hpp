#pragma once

#include "Engine/Core.hpp"

#include <span>
#include <string>
#include <string_view>

namespace orc {

class Shader
{
public:
	Shader() = default;
	Shader(const FilePath& vertexFilePath, const FilePath& fragmentFilePath);
	~Shader();

	Shader(Shader&&) = delete;
	Shader(const Shader&) = delete;
	Shader operator=(Shader&&) = delete;
	Shader operator=(const Shader&) = delete;

	bool loadFromFile(const FilePath& vertexFilePath, const FilePath& fragmentFilePath);
	bool loadFromString(std::string_view vertexShaderSource, std::string_view fragmentShaderSource);

	void bind() const;
	void unbind() const;

	void uploadUniformInt(const std::string& name, int integer) const;
	void uploadUniformIntArray(const std::string& name, const int* array, uint32_t size) const;

	template<typename T>
	void uploadUniformIntArray(const std::string& name, std::span<T> array) const
	{
		uploadUniformIntArray(name, array.data(), static_cast<uint32_t>(array.size()));
	}

	void uploadUniformFloat3(const std::string& name, const Vector3f& float3) const;
	void uploadUniformFloat4(const std::string& name, const Vector4f& float4) const;
	void uploadUniformMatrix4(const std::string& name, const Matrix4& matrix) const;
	void uploadUniformMatrix3(const std::string& name, const Matrix& matrix) const;

private:
	bool readShader(std::string* shader, const FilePath& filePath);
	bool compile(std::string_view vertexShaderSource, std::string_view fragmentShaderSource);

	RendererID m_rendererID = 0;
};

}
