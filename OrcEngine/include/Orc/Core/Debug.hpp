#pragma once

#include <cstdlib>

#include "Core/Platform.hpp"
#include "Core/Logger.hpp"

#define ORC_FATAL(message, ...) \
	do \
	{ \
		ORC_LOG_FATAL(message __VA_OPT__(, ) __VA_ARGS__); \
		ORC_DEBUGBREAK(); \
		std::abort(); \
	} while (0)

#define ORC_FATAL_CHECK(check, message, ...) \
	do \
	{ \
		if (!(check)) \
		{ \
			ORC_FATAL(message __VA_OPT__(, ) __VA_ARGS__); \
		} \
	} while (0)
