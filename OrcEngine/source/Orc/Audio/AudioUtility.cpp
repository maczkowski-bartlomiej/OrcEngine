#include "OrcPch.hpp"

#include "AudioUtility.hpp"

#include <fmod_common.h>
#include <fmod_errors.h>

#include "Engine/Logger.hpp"

namespace orc {

bool fmodCall(FMOD_RESULT result, std::source_location sourceLocation)
{
	if (result != FMOD_RESULT::FMOD_OK)
	{
		Logger::log(Logger::Level::Error, sourceLocation, "FMOD Error: {}", FMOD_ErrorString(result));
		return false;
	}

	return true;
}

} // namespace orc
