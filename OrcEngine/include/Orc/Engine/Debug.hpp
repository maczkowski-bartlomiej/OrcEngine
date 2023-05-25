#pragma once

#include "Engine/Core.hpp"
#include "Engine/Logger.hpp"

#ifdef ORC_DEBUG_ASSERTS
	#define ORC_ASSERT(check, message, ...) { if(!(check)) { ORC_LOG_FATAL(message, __VA_ARGS__); ORC_DEBUGBREAK(); } }

	#define ORC_ERROR(message, ...) ORC_LOG_ERROR(message, __VA_ARGS__); ORC_DEBUGBREAK();
	#define ORC_FATAL(message, ...) ORC_LOG_FATAL(message, __VA_ARGS__); ORC_DEBUGBREAK();

	#define ORC_ERROR_CHECK(check, message, ...) { if(!(check)) { ORC_LOG_ERROR(message, __VA_ARGS__); ORC_DEBUGBREAK(); } }
	#define ORC_FATAL_CHECK(check, message, ...) { if(!(check)) { ORC_LOG_FATAL(message, __VA_ARGS__); ORC_DEBUGBREAK(); } }

#else
	#define ORC_ASSERT(check, message, ...)

	#define ORC_ERROR(message, ...) ORC_LOG_ERROR(message, __VA_ARGS__);
	#define ORC_FATAL(message, ...) ORC_LOG_FATAL(message, __VA_ARGS__);

	#define ORC_ERROR_CHECK(check, message, ...) { if(!(check)) { ORC_LOG_ERROR(message, __VA_ARGS__); } }
	#define ORC_FATAL_CHECK(check, message, ...) { if(!(check)) { ORC_LOG_FATAL(message, __VA_ARGS__); } }
#endif

