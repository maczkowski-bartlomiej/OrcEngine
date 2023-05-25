#pragma once

#include <string_view>
#include <unordered_map>
#include <unordered_set>

namespace orc {
namespace utility {

struct StringHash
{
    auto operator()(std::string_view string) const
    {
        return std::hash<std::string_view>()(string);
    }
};

struct StringEqual
{
    bool operator()(std::string_view left, std::string_view right) const
    {
        return left == right;
    }
};

using StringViewUnorderedSet = std::unordered_set<std::string, StringHash, StringEqual>;
template <typename Value>
using StringViewUnorderedMap = std::unordered_map<std::string, Value, StringHash, StringEqual>;

}
}

inline std::string operator+(const char* a, std::string_view b)
{
    return a + std::string(b);
}

inline std::string operator+(std::string& a, std::string_view b)
{
    return a + b;
}
