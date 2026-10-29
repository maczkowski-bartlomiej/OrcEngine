#pragma once

#include "Core/Types.hpp"

namespace orc {

template <typename T>
struct Rect
{
	Rect()
		: left(T{}), top(T{}), width(T{}), height(T{})
	{
	}

	Rect(T left, T top, T width, T height)
		: left(left), top(top), width(width), height(height)
	{
	}

	template <typename U>
	explicit Rect(const Rect<U>& other)
		: left(static_cast<T>(other.left)), top(static_cast<T>(other.top)), width(static_cast<T>(other.width)), height(static_cast<T>(other.height))
	{
	}

	Vector2<T> getPosition() const
	{
		return Vector2<T>(left, top);
	}

	Vector2<T> getSize() const
	{
		return Vector2<T>(width, height);
	}

	bool intersects(const Rect& other) const
	{
		return left < other.left + other.width && left + width > other.left && top < other.top + other.height && top + height > other.top;
	}

	T left, top, width, height;
};

using IntRect = Rect<int>;
using FloatRect = Rect<float>;

}
