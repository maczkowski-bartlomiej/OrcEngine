#pragma once

#include <cstdint>
#include <filesystem>

#define GLM_FORCE_CTOR_INIT
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/detail/qualifier.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

namespace orc {

using FilePath = std::filesystem::path;
using RendererID = uint32_t;

template <typename T>
using Vector2 = glm::tvec2<T>;
using Vector2i = glm::tvec2<int>;
using Vector2b = glm::tvec2<bool>;
using Vector2f = glm::tvec2<float>;
using Vector2d = glm::tvec2<double>;
using Vector2u = glm::tvec2<unsigned int>;

template <typename T>
using Vector3 = glm::tvec3<T>;
using Vector3i = glm::tvec3<int>;
using Vector3b = glm::tvec3<bool>;
using Vector3f = glm::tvec3<float>;
using Vector3d = glm::tvec3<double>;
using Vector3u = glm::tvec3<unsigned int>;

template <typename T>
using Vector4 = glm::tvec4<T>;
using Vector4i = glm::tvec4<int>;
using Vector4b = glm::tvec4<bool>;
using Vector4f = glm::tvec4<float>;
using Vector4d = glm::tvec4<double>;
using Vector4u = glm::tvec4<unsigned int>;

using Matrix = glm::mat3;
using Matrix4 = glm::mat4;

}
