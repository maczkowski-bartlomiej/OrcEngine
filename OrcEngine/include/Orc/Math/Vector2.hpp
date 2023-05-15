#pragma once

#include "System/System.hpp"

namespace orc {

template<typename T>
class Vector2
{
public:
	inline Vector2(T x = {}, T y = {}) 
		: x(x), y(y) {}

	template<typename Other>
	inline explicit Vector2(const Vector2<Other>& vector) 
		: x(static_cast<T>(vector.x)), y(static_cast<T>(vector.y)) {}

	inline Vector2<T> operator-() 
	{
		return Vector2<T>(-x, -y);
	}

	inline Vector2<T>& operator+=(const Vector2<T>& vector) 
	{
		x += vector.x;
		y += vector.y;

		return *this;
	}

	inline Vector2<T>& operator-=(const Vector2<T>& vector) 
	{
		x -= vector.x;
		y -= vector.y;

		return *this;
	}

	inline Vector2<T>& operator*=(const Vector2<T>& vector) 
	{
		x *= vector.x;
		y *= vector.y;

		return *this;
	}

	inline Vector2<T>& operator*=(float scalar) 
	{
		x *= scalar;
		y *= scalar;

		return *this;
	}

	inline Vector2<T>& operator/=(float scalar) 
	{
		x /= scalar;
		y /= scalar;

		return *this;
	}

	inline bool operator==(const Vector2<T>& vector) const 
	{
		return x == vector.x && y == vector.y;
	}

	inline bool operator!=(const Vector2<T>& vector) const 
	{
		return x != vector.x || y != vector.y;
	}

	T x;
	T y;
};

	using Vector2i = Vector2<int32>;
	using Vector2l = Vector2<int64>;
	using Vector2u = Vector2<uint32>;
	using Vector2b = Vector2<bool>;
	using Vector2f = Vector2<float>;
}

template<typename T>
inline orc::Vector2<T> operator+(const orc::Vector2<T>& vector1, const orc::Vector2<T>& vector2) 
{
	return Vector2<T>(vector1.x + vector2.x, vector1.y + vector2.y);
}

template<typename T>
inline orc::Vector2<T> operator-(const orc::Vector2<T>& vector1, const orc::Vector2<T>& vector2) 
{
	return Vector2<T>(vector1.x - vector2.x, vector1.y - vector2.y);
}

template<typename T>
inline orc::Vector2<T> operator*(const orc::Vector2<T>& vector1, const orc::Vector2<T>& vector2) 
{
	return Vector2<T>(vector1.x * vector2.x, vector1.y * vector2.y);
}

template<typename T>
inline orc::Vector2<T> operator/(const orc::Vector2<T>& vector1, const orc::Vector2<T>& vector2) 
{
	return Vector2<T>(vector1.x / vector2.x, vector1.y / vector2.y);
}

template<typename T>
inline orc::Vector2<T> operator*(const orc::Vector2<T>& vector, float scalar) 
{
	return Vector2<T>(vector.x * scalar, vector.y * scalar);
}

template<typename T>
inline orc::Vector2<T> operator/(const orc::Vector2<T>& vector, float scalar) 
{
	return Vector2<T>(vector.x / scalar, vector.y / scalar);
}

template<typename T>
inline orc::Vector2<T> operator*(float scalar, const orc::Vector2<T>& vector) 
{
	return Vector2<T>(vector.x * scalar, vector.y * scalar);
}
