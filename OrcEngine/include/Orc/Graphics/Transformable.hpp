#pragma once

#include "Engine/Core.hpp"

namespace orc {
	
class Transformable
{
public:
	Transformable();
	virtual ~Transformable() = default;

	void rotate(float angle);

	void scale(float value);
	void scale(float x, float y);
	void scale(const Vector2f& size);

	void move(float x, float y);
	void move(const Vector2f& offset);

	void setRotation(float angle);

	void setOrigin(float x, float y);
	void setOrigin(const Vector2f& origin);

	void setScale(float value);
	void setScale(float x, float y);
	void setScale(const Vector2f& size);	

	void setPosition(float x, float y);
	void setPosition(const Vector2f& position);

	float getRotation() const;
	Vector2f getScale() const;
	Vector2f getOrigin() const;
	Vector2f getPosition() const;

	const Matrix& getTransformMatrix() const;
	const Matrix& getInverseTransformMatrix() const;

protected:
	virtual void onTransformChangeCallback() {}

	float m_rotation;
	Vector2f m_scale;
	Vector2f m_origin;
	Vector2f m_position;

	mutable Matrix m_transformMatrix;
	mutable Matrix m_inverseTransformMatrix;

	mutable bool m_isTransformMatrixUpdateNeeded;
	mutable bool m_isInverseTransformMatrixUpdateNeeded;
};

}
