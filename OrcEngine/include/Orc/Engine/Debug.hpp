#pragma once

#include "Engine/Core.hpp"

#include "Logger.hpp"
#include <cstdlib>
#include <fmt/format.h>
#include <source_location>

#ifdef ORC_DEBUG_ASSERTS

#define ORC_ASSERT(condition, message, ...) do {    \
    if ( !(condition) ) {                           \
        ORC_LOG_FATAL(message, __VA_ARGS__);		\
		ORC_DEBUGBREAK();                           \
        std::abort();                               \
    }                                               \
} while(0)

#define ORC_WARNING(message, ...) { ORC_LOG_WARNING(message, __VA_ARGS__); ORC_DEBUGBREAK(); }
#define ORC_ERROR(message, ...) { ORC_LOG_ERROR(message, __VA_ARGS__); ORC_DEBUGBREAK(); }
#define ORC_FATAL(message, ...) { ORC_LOG_FATAL(message, __VA_ARGS__); ORC_DEBUGBREAK(); }
#define ORC_ERROR_CHECK(check, message, ...) { if(!(check)) { ORC_LOG_ERROR(message, __VA_ARGS__); ORC_DEBUGBREAK(); } }
#define ORC_FATAL_CHECK(check, message, ...) { if(!(check)) { ORC_LOG_FATAL(message, __VA_ARGS__); ORC_DEBUGBREAK(); } }

#else

#define ORC_ASSERT(condition, message, ...)

#define ORC_WARNING(message, ...) { ORC_LOG_WARNING(message, __VA_ARGS__); }
#define ORC_ERROR(message, ...) { ORC_LOG_ERROR(message, __VA_ARGS__); }
#define ORC_FATAL(message, ...) { ORC_LOG_FATAL(message, __VA_ARGS__); }
#define ORC_ERROR_CHECK(check, message, ...) { if(!(check)) { ORC_LOG_ERROR(message, __VA_ARGS__); } }
#define ORC_FATAL_CHECK(check, message, ...) { if(!(check)) { ORC_LOG_FATAL(message, __VA_ARGS__); } }

#endif
