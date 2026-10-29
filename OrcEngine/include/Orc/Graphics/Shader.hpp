#pragma once

#include <span>
#include <string>
#include <string_view>

#include "Core/Types.hpp"

namespace orc {

class Shader
{
public:
	Shader() = default;
	Shader(const FilePath& vertexFilePath, const FilePath& fragmentFilePath);
	~Shader();

	Shader(Shader&& other) noexcept;
	Shader& operator=(Shader&& other) noexcept;
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	void loadFromFile(const FilePath& vertexFilePath, const FilePath& fragmentFilePath);
	void loadFromString(std::string_view vertexSource, std::string_view fragmentSource);

	void bind() const;
	void unbind() const;

	void uploadUniformInt(const std::string& name, int integer) const;
	void uploadUniformIntArray(const std::string& name, std::span<const int> values) const;

	void uploadUniformFloat3(const std::string& name, const Vector3f& float3) const;
	void uploadUniformFloat4(const std::string& name, const Vector4f& float4) const;
	void uploadUniformMatrix4(const std::string& name, const Matrix4& matrix) const;
	void uploadUniformMatrix3(const std::string& name, const Matrix& matrix) const;

private:
	void readShader(std::string* shader, const FilePath& filePath);
	void compile(std::string_view vertexSource, std::string_view fragmentSource);

	RendererID m_rendererID = 0;
};

}
