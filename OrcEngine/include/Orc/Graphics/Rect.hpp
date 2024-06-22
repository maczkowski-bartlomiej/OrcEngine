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

	Vector2<T> getPosition() const
	{
		return Vector2<T>(x, y);
	}

	Vector2<T> getSize() const
	{
		return Vector2<T>(width, height);
	}

	bool intersects(const Rect<T>& other) const
	{
		return x < other.x + other.width && x + width > other.x && y < other.y + other.height && y + height > other.y;
	}

	T x, y, width, height;
};

using IntRect = Rect<int>;
using FloatRect = Rect<float>;

}
