#pragma once

#include <memory>
#include <utility>

namespace orc {

template <typename T>
using Ref = std::shared_ptr<T>;
template <typename T>
using UniquePtr = std::unique_ptr<T>;

template <typename T, typename... Arguments>
constexpr Ref<T> createRef(Arguments&&... arguments)
{
	return std::make_shared<T>(std::forward<Arguments>(arguments)...);
}

template <typename T, typename... Arguments>
constexpr UniquePtr<T> createUniquePtr(Arguments&&... arguments)
{
	return std::make_unique<T>(std::forward<Arguments>(arguments)...);
}

} // namespace orc
