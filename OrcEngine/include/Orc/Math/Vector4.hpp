#pragma once

#include "System/System.hpp"

namespace orc {

template<typename T>
class Vector4
{
public:
	inline Vector4(T x = {}, T y = {}, T z = {}, T p = {}) 
		: x(x), y(y), z(z), p(p) {}

	template<typename Other>
	inline explicit Vector4(const Vector4<Other>& vector) 
		: x(static_cast<T>(vector.x)), y(static_cast<T>(vector.y)), z(static_cast<T>(vector.z)), p(static_cast<T>(vector.p)) {}

	inline Vector4<T> operator-() 
	{
		return Vector4<T>(-x, -y, -z, -p);
	}

	inline Vector4<T>& operator+=(const Vector4<T>& vector) 
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
		p += vector.p;

		return *this;
	}

	inline Vector4<T>& operator-=(const Vector4<T>& vector) 
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		p -= vector.p;

		return *this;
	}

	inline Vector4<T>& operator*=(const Vector4<T>& vector) 
	{
		x *= vector.x;
		y *= vector.y;
		z *= vector.z;
		p *= vector.p;

		return *this;
	}

	inline Vector4<T>& operator*=(float scalar) 
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		p *= scalar;

		return *this;
	}

	inline Vector4<T>& operator/=(float scalar) 
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		p /= scalar;

		return *this;
	}

	inline bool operator==(const Vector4<T>& vector) const 
	{
		return x == vector.x && y == vector.y && z == vector.z && p == vector.p;
	}

	inline bool operator!=(const Vector4<T>& vector) const 
	{
		return x != vector.x || y != vector.y || z != vector.z || p != vector.p;
	}

	T x;
	T y;
	T z;
	T p;
};
	
	using Vector4i = Vector4<int32>;
	using Vector4l = Vector4<int64>;
	using Vector4u = Vector4<uint32>;
	using Vector4b = Vector4<bool>;
	using Vector4f = Vector4<float>;
}

template<typename T>
inline orc::Vector4<T> operator+(const orc::Vector4<T>& vector1, const orc::Vector4<T>& vector2) 
{
	return Vector4<T>(vector1.x + vector2.x, vector1.y + vector2.y, vector1.z + vector2.z, vector1.p + vector2.p);
}

template<typename T>
inline orc::Vector4<T> operator-(const orc::Vector4<T>& vector1, const orc::Vector4<T>& vector2) 
{
	return Vector4<T>(vector1.x - vector2.x, vector1.y - vector2.y, vector1.z - vector2.z, vector1.p - vector2.p);
}

template<typename T>
inline orc::Vector4<T> operator*(const orc::Vector4<T>& vector1, const orc::Vector4<T>& vector2) 
{
	return Vector4<T>(vector1.x * vector2.x, vector1.y * vector2.y, vector1.z * vector2.z, vector1.p * vector2.p);
}

template<typename T>
inline orc::Vector4<T> operator/(const orc::Vector4<T>& vector1, const orc::Vector4<T>& vector2) 
{
	return Vector4<T>(vector1.x / vector2.x, vector1.y / vector2.y, vector1.z / vector2.z, vector1.p / vector2.p);
}

template<typename T>
inline orc::Vector4<T> operator*(const orc::Vector4<T>& vector, float scalar) 
{
	return Vector4<T>(vector.x * scalar, vector.y * scalar, vector.z * scalar, vector.p * scalar);
}

template<typename T>
inline orc::Vector4<T> operator/(const orc::Vector4<T>& vector, float scalar) 
{
	return Vector4<T>(vector.x / scalar, vector.y / scalar, vector.z / scalar, vector.p / scalar);
}

template<typename T>
inline orc::Vector4<T> operator*(float scalar, const orc::Vector4<T>& vector) 
{
	return Vector4<T>(vector.x * scalar, vector.y * scalar, vector.z * scalar, vector.p * scalar);
}
