#pragma once

#include "Engine/Logger.hpp"

#include <fmod_errors.h>

#define FMOD_CALL(result) fmodCall(std::source_location::current(), result)

namespace orc {

static bool fmodCall(std::source_location sourceLocation, FMOD_RESULT result)
{
    if (result != FMOD_RESULT::FMOD_OK)
    {
        Logger::log(Logger::Level::Error, sourceLocation, "FMOD Error: {}", FMOD_ErrorString(result));
        return false;
    }

    return true;
}

}
