#pragma once

#include "Tools/Logger.hpp"
#include "System/System.hpp"

#ifdef ORC_DEBUG_ASSERTS
	#define ORC_ASSERT(check, message, ...) { if(!(check)) { ORC_LOG_FATAL(message, __VA_ARGS__); ORC_DEBUGBREAK(); } }
	#define ORC_CORE_ASSERT(check, message, ...) { if(!(check)) { ORC_CORE_LOG_FATAL(message, __VA_ARGS__); ORC_DEBUGBREAK(); } }
#else
	#define ORC_ASSERT(check, message, ...)
	#define ORC_CORE_ASSERT(check, message, ...)
#endif

#ifdef ORC_DEBUG_EXCEPTIONS
	#define ORC_THROW_RUNTIME_ERROR_NO_LOG(message) { throw std::runtime_error(message); }

	#define ORC_THROW_RUNTIME_ERROR(message, ...) { ORC_LOG_FATAL(message, __VA_ARGS__); throw std::runtime_error(""); }
	#define ORC_CORE_THROW_RUNTIME_ERROR(message, ...) { ORC_CORE_LOG_FATAL(message, __VA_ARGS__); throw std::runtime_error(""); }

	#define ORC_THROW_RUNTIME_ERROR_CHECK(check, message, ...) { if(!(check)) { ORC_LOG_FATAL(message, __VA_ARGS__); throw std::runtime_error(""); } }
	#define ORC_CORE_THROW_RUNTIME_ERROR_CHECK(check, message, ...) { if(!(check)) { ORC_CORE_LOG_FATAL(message, __VA_ARGS__); throw std::runtime_error(""); } }

	#define ORC_THROW_RUNTIME_ERROR_CHECK_NO_LOG(check, message) { if(!(check)) { throw std::runtime_error(message); } }
#else
	#define ORC_THROW_RUNTIME_ERROR_NO_LOG(message)

	#define ORC_THROW_RUNTIME_ERROR(message, ...)
	#define ORC_CORE_THROW_RUNTIME_ERROR(message, ...)

	#define ORC_THROW_RUNTIME_ERROR_CHECK(check, message, ...)
	#define ORC_CORE_THROW_RUNTIME_ERROR_CHECK(check, message, ...)

	#define ORC_THROW_RUNTIME_ERROR_CHECK_NO_LOG(check, message)
#endif
