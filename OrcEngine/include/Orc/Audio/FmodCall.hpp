#pragma once

#include <source_location>

#define FMOD_CALL(result) orc::fmodCall(std::source_location::current(), result)

enum FMOD_RESULT;

namespace orc {

	bool fmodCall(std::source_location sourceLocation, FMOD_RESULT result);

}
