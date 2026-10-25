#pragma once

#include <cstdlib>
#include <source_location>

#include <fmt/format.h>

#include "Core/Platform.hpp"

#include "Logger.hpp"


#ifdef ORC_DEBUG_ASSERTS

#define ORC_FATAL(message, ...) \
	do \
	{ \
		ORC_LOG_FATAL(message __VA_OPT__(,) __VA_ARGS__); \
		ORC_DEBUGBREAK(); \
		std::abort(); \
	} while (0)

#define ORC_FATAL_CHECK(check, message, ...) \
	do \
	{ \
		if (!(check)) \
		{ \
			ORC_LOG_FATAL(message __VA_OPT__(,) __VA_ARGS__); \
			ORC_DEBUGBREAK(); \
			std::abort(); \
		} \
	} while (0)

#else

#define ORC_FATAL(message, ...) \
	do \
	{ \
		ORC_LOG_FATAL(message __VA_OPT__(,) __VA_ARGS__); \
		std::abort(); \
	} while (0)

#define ORC_FATAL_CHECK(check, message, ...) \
	do \
	{ \
		if (!(check)) \
		{ \
			ORC_LOG_FATAL(message __VA_OPT__(,) __VA_ARGS__); \
			std::abort(); \
		} \
	} while (0)

#endif
