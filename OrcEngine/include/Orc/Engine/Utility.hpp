#pragma once

#include <source_location>
#include <string>
#include <string_view>

#include <freetype/freetype.h>
#include <freetype/fterrors.h>
#include <freetype/ftimage.h>
#include <freetype/fttypes.h>

namespace orc {

struct string_view_hash
{
	using hash_type = std::hash<std::string_view>;
	using is_transparent = void;

	std::size_t operator()(const char* str) const { return hash_type{}(str); }
	std::size_t operator()(std::string_view str) const { return hash_type{}(str); }
	std::size_t operator()(std::string const& str) const { return hash_type{}(str); }
};

bool ftCall(FT_Error result, std::source_location sourceLocation = std::source_location::current());

std::string getErrnoMessage(int error);

} // namespace orc
