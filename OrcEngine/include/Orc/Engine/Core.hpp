#pragma once

#include <memory>
#include <cstdint>
#include <filesystem>

#define GLM_FORCE_CTOR_INIT
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#ifdef ORC_DEBUG
	#define ORC_DEBUG_ASSERTS

	#ifdef ORC_PLATFORM_WINDOWS
		#define ORC_DEBUGBREAK() __debugbreak()
	#elif defined ORC_PLATFORM_LINUX
		#include <signal.h>
		#define ORC_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak!"
	#endif

#endif

#ifdef ORC_RELEASE
#endif

#ifdef ORC_DISTRIBUTION
#endif

namespace orc {

using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

using FilePath = std::filesystem::path;

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T>
using UniquePtr = std::unique_ptr<T>;

template<typename T, typename... Arguments>
constexpr Ref<T> createRef(Arguments&&... arguments)
{
	return std::make_shared<T>(std::forward<Arguments>(arguments)...);
}

template<typename T, typename... Arguments>
constexpr UniquePtr<T> createUniquePtr(Arguments&&... arguments)
{
	return std::make_unique<T>(std::forward<Arguments>(arguments)...);
}

using RendererID = uint32;

template<typename T>
using Vector2 = glm::tvec2<T>;
using Vector2i = glm::tvec2<int>;
using Vector2b = glm::tvec2<bool>;
using Vector2f = glm::tvec2<float>;
using Vector2d = glm::tvec2<double>;
using Vector2u = glm::tvec2<unsigned int>;

template<typename T>
using Vector3 = glm::tvec3<T>;
using Vector3i = glm::tvec3<int>;
using Vector3b = glm::tvec3<bool>;
using Vector3f = glm::tvec3<float>;
using Vector3d = glm::tvec3<double>;
using Vector3u = glm::tvec3<unsigned int>;

template<typename T>
using Vector4 = glm::tvec4<T>;
using Vector4i = glm::tvec4<int>;
using Vector4b = glm::tvec4<bool>;
using Vector4f = glm::tvec4<float>;
using Vector4d = glm::tvec3<double>;
using Vector4u = glm::tvec4<unsigned int>;

using Matrix = glm::mat3;
using Matrix4 = glm::mat4;

namespace version {
	static inline constexpr uint32 MAJOR_VERSION = 0u;
	static inline constexpr uint32 MINOR_VERSION = 0u;
	static inline constexpr uint32 PATCH_VERSION = 1u;
}

}
