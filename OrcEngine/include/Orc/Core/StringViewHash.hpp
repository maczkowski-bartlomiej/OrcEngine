#pragma once

#include <cstddef>
#include <string>
#include <string_view>

namespace orc {

struct StringViewHash
{
	using HashType = std::hash<std::string_view>;
	using is_transparent = void;

	std::size_t operator()(const char* str) const
	{
		return HashType{}(str);
	}

	std::size_t operator()(std::string_view str) const
	{
		return HashType{}(str);
	}

	std::size_t operator()(const std::string& str) const
	{
		return HashType{}(str);
	}
};

}
