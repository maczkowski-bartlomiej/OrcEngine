#pragma once

#include "Core/Types.hpp"

namespace orc {

template <typename T>
struct Rect
{
public:
	Rect()
		: left(T{}), top(T{}), right(T{}), bottom(T{}) {}

	Rect(T x, T y, T width, T height)
		: left(x), top(y), right(width), bottom(height) {}

	template <typename U>
	explicit Rect(const Rect<U>& other)
		: left(static_cast<T>(other.left)), top(static_cast<T>(other.top)), right(static_cast<T>(other.right)), bottom(static_cast<T>(other.bottom)) {}

	Vector2<T> getPosition() const
	{
		return Vector2<T>(left, top);
	}

	Vector2<T> getSize() const
	{
		return Vector2<T>(right, bottom);
	}

	bool intersects(const Rect<T>& other) const
	{
		return left < other.left + other.right && left + right > other.left && top < other.top + other.bottom && top + bottom > other.top;
	}

	T left, top, right, bottom;
};

using IntRect = Rect<int>;
using FloatRect = Rect<float>;

}
