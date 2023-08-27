#pragma once

#include "Engine/Core.hpp"

namespace orc {

template<typename T>
struct Rect
{
public:
	Rect()
		: x(T{}), y(T{}), width(T{}), height(T{}) {}

	Rect(T x, T y, T width, T height)
		: x(x), y(y), width(width), height(height) {}

	template<typename U>
	Rect(const Rect<U>& other)
		: x((T)other.x), y((T)other.y), width((T)other.width), height((T)other.height) {}

	T x, y, width, height;
};

using IntRect = Rect<int>;
using FloatRect = Rect<float>;

}
