#pragma once

#include <freetype/freetype.h>

#include "Engine/Debug.hpp"

#define ORC_FT_CALL(call, message, ...) \
	do \
	{ \
		const FT_Error ftResult = (call); \
		ORC_FATAL_CHECK(ftResult == FT_Err_Ok, message "\n\tFreeType Error: {}" __VA_OPT__(, ) __VA_ARGS__, FT_Error_String(ftResult)); \
	} while (0)
