#pragma once

#include <memory>
#include <cstdint>
#include <filesystem>

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

using FilePath = std::filesystem::path;

template<typename T>
using Reference = std::shared_ptr<T>;

template<typename T>
using UniquePointer = std::unique_ptr<T>;

template<typename T, typename... Arguments>
constexpr Reference<T> createReference(Arguments&&... arguments)
{
	return std::make_shared<T>(std::forward<Arguments>(arguments)...);
}

template<typename T, typename... Arguments>
constexpr UniquePointer<T> createUniquePointer(Arguments&&... arguments)
{
	return std::make_unique<T>(std::forward<Arguments>(arguments)...);
}

using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

using RendererID = uint32;

}
