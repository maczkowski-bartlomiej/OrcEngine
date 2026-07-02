#pragma once

#include "Engine/Core.hpp"

#include "Logger.hpp"
#include <cstdlib>
#include <fmt/format.h>
#include <source_location>
#include <Windows.h>

#ifdef ORC_DEBUG_ASSERTS

#define ORC_ASSERT(condition, message, ...) do {    \
    if ( !(condition) ) {                           \
        ORC_LOG_FATAL(message, __VA_ARGS__);		\
		ORC_DEBUGBREAK();                           \
        std::abort();                               \
    }                                               \
} while(0)

#define ORC_LOG_IF(condition, msg, ...) \
([&](){ \
    if (condition) { \
        ORC_LOG_ERROR(msg, __VA_ARGS__); \
        ORC_DEBUGBREAK(); \
        return false; \
    } \
    return true; \
}())

#endif

#ifndef ORC_DEBUG_ASSERTS
#define ORC_ASSERT(condition, message, ...)

#define ORC_CHECK(condition, message, ...) do {     \
		if ( !(condition) ) {                           \
			ORC_LOG_ERROR(message, __VA_ARGS__);		\
		}                                               \
	} while(0)
#endif



#ifdef ORC_DEBUG_ASSERTS

#define ORC_WARNING(message, ...) { ORC_LOG_WARNING(message, __VA_ARGS__); ORC_DEBUGBREAK(); }
#define ORC_ERROR(message, ...) { ORC_LOG_ERROR(message, __VA_ARGS__); ORC_DEBUGBREAK(); }
#define ORC_FATAL(message, ...) { ORC_LOG_FATAL(message, __VA_ARGS__); ORC_DEBUGBREAK(); }
#define ORC_ERROR_CHECK(check, message, ...) { if(!(check)) { ORC_LOG_ERROR(message, __VA_ARGS__); ORC_DEBUGBREAK(); } }
#define ORC_FATAL_CHECK(check, message, ...) { if(!(check)) { ORC_LOG_FATAL(message, __VA_ARGS__); ORC_DEBUGBREAK(); } }

#define ORC_CHECK_IMPL(level, action, check, message, ...) \
		do { \
			if (!(check)) { \
				ORC_LOG_##level(message, __VA_ARGS__); \
				ORC_DEBUGBREAK(); \
				action; \
			} \
		} while(0)


#define ORC_CHECK_WARNING(check, message, ...) ORC_CHECK_IMPL(WARNING, , check, message, __VA_ARGS__)
#define ORC_CHECK_ERROR(check, message, ...)   ORC_CHECK_IMPL(ERROR, , check, message, __VA_ARGS__)
#define ORC_CHECK_FATAL(check, message, ...)   ORC_CHECK_IMPL(FATAL, , check, message, __VA_ARGS__)

#define ORC_CHECK_THROW_ERROR(check, message, ...) ORC_CHECK_IMPL(ERROR, throw orc::OrcException(fmt::format(message, __VA_ARGS__), std::source_location::current()), check, message, __VA_ARGS__)
#define ORC_CHECK_THROW_FATAL(check, message, ...) ORC_CHECK_IMPL(FATAL, throw orc::OrcException(fmt::format(message, __VA_ARGS__), std::source_location::current()), check, message, __VA_ARGS__)

#endif
