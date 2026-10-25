#pragma once

#include <fmod_common.h>
#include <fmod_errors.h>

#include "Engine/Debug.hpp"

#define ORC_FMOD_CALL(call, message, ...) \
	do \
	{ \
		const FMOD_RESULT fmodResult = (call); \
		ORC_FATAL_CHECK(fmodResult == FMOD_RESULT::FMOD_OK, message "\n\tFMOD Error: {}" __VA_OPT__(, ) __VA_ARGS__, FMOD_ErrorString(fmodResult)); \
	} while (0)
