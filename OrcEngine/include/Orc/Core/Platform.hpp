#pragma once

#if !defined(ORC_PLATFORM_WINDOWS) && !defined(ORC_PLATFORM_LINUX)
	#if defined(_WIN32) || defined(_WIN64)
		#define ORC_PLATFORM_WINDOWS
	#elif defined(__linux__) || defined(__gnu_linux__)
		#define ORC_PLATFORM_LINUX
	#else
		#error "Unknown or unsupported platform!"
	#endif
#endif

#if defined(_MSC_VER)
#include <intrin.h>
#endif

#ifdef ORC_DEBUG
#define ORC_DEBUG_ASSERTS

#if defined(ORC_PLATFORM_WINDOWS)
#define ORC_DEBUGBREAK() __debugbreak()
#elif defined(ORC_PLATFORM_LINUX)
#include <signal.h>
#define ORC_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak!"
#endif

#endif

#ifdef ORC_RELEASE
#define ORC_DEBUGBREAK()
#endif
