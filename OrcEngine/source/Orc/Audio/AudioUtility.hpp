#pragma once

#include <source_location>
#include <fmod_common.h>

namespace orc {

bool fmodCall(FMOD_RESULT result, std::source_location sourceLocation = std::source_location::current());

} // namespace orc
