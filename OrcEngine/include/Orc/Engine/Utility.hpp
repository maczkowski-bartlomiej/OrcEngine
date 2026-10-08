#pragma once

#include <string>
#include <string_view>

#include <fmod_common.h>

#include <freetype/freetype.h>
#include <freetype/fterrors.h>
#include <freetype/fttypes.h>
#include <freetype/ftimage.h>

namespace orc {

struct string_view_hash
{
    using hash_type = std::hash<std::string_view>;
    using is_transparent = void;

    std::size_t operator()(const char* str) const { return hash_type{}(str); }
    std::size_t operator()(std::string_view str) const { return hash_type{}(str); }
    std::size_t operator()(std::string const& str) const { return hash_type{}(str); }
};

[[nodiscard]] bool fmodCall(FMOD_RESULT result, std::source_location sourceLocation = std::source_location::current());
[[nodiscard]] bool ftCall(FT_Error result, std::source_location sourceLocation = std::source_location::current());

std::string getErrnoMessage(int error);

}
