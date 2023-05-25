#pragma once

#include "Engine/Core.hpp"

namespace orc {

template<typename T>
class Vector3
{
public:
	inline Vector3(T x = {}, T y = {}, T z = {}) 
		: x(x), y(y), z(z) {}

	template<typename Other>
	inline explicit Vector3(const Vector3<Other>& vector) 
		: x(static_cast<T>(vector.x)), y(static_cast<T>(vector.y)), z(static_cast<T>(vector.z)) {}

	inline Vector3<T> operator-() 
	{
		return Vector3<T>(-x, -y, -z);
	}

	inline Vector3<T>& operator+=(const Vector3<T>& vector) 
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;

		return *this;
	}

	inline Vector3<T>& operator-=(const Vector3<T>& vector) 
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;

		return *this;
	}

	inline Vector3<T>& operator*=(const Vector3<T>& vector) 
	{
		x *= vector.x;
		y *= vector.y;
		z *= vector.z;

		return *this;
	}

	inline Vector3<T>& operator*=(float scalar) 
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;

		return *this;
	}

	inline Vector3<T>& operator/=(float scalar) 
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;

		return *this;
	}

	inline bool operator==(const Vector3<T>& vector) const 
	{
		return x == vector.x && y == vector.y && z == vector.z;
	}

	inline bool operator!=(const Vector3<T>& vector) const 
	{
		return x != vector.x || y != vector.y || z != vector.z;
	}

	T x;
	T y;
	T z;
};

	using Vector3i = Vector3<int32>;
	using Vector3l = Vector3<int64>;
	using Vector3u = Vector3<uint32>;
	using Vector3b = Vector3<bool>;
	using Vector3f = Vector3<float>;
}

template<typename T>
inline orc::Vector3<T> operator+(const orc::Vector3<T>& vector1, const orc::Vector3<T>& vector2) 
{
	return Vector3<T>(vector1.x + vector2.x, vector1.y + vector2.y, vector1.z + vector2.z);
}

template<typename T>
inline orc::Vector3<T> operator-(const orc::Vector3<T>& vector1, const orc::Vector3<T>& vector2) 
{
	return Vector3<T>(vector1.x - vector2.x, vector1.y - vector2.y, vector1.z - vector2.z);
}

template<typename T>
inline orc::Vector3<T> operator*(const orc::Vector3<T>& vector1, const orc::Vector3<T>& vector2) 
{
	return Vector3<T>(vector1.x * vector2.x, vector1.y * vector2.y, vector1.z * vector2.z);
}

template<typename T>
inline orc::Vector3<T> operator/(const orc::Vector3<T>& vector1, const orc::Vector3<T>& vector2) 
{
	return Vector3<T>(vector1.x / vector2.x, vector1.y / vector2.y, vector1.z / vector2.z);
}

template<typename T>
inline orc::Vector3<T> operator*(const orc::Vector3<T>& vector, float scalar) 
{
	return Vector3<T>(vector.x * scalar, vector.y * scalar, vector.z * scalar);
}

template<typename T>
inline orc::Vector3<T> operator/(const orc::Vector3<T>& vector, float scalar) 
{
	return Vector3<T>(vector.x / scalar, vector.y / scalar, vector.z / scalar);
}

template<typename T>
inline orc::Vector3<T> operator*(float scalar, const orc::Vector3<T>& vector) 
{
	return Vector3<T>(vector.x * scalar, vector.y * scalar, vector.z * scalar);
}
