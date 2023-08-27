#pragma once

#include "Engine/Core.hpp"

namespace orc {
	
class Transformable
{
public:
	Transformable();
	virtual ~Transformable() = default;

	void rotate(float angle);

	void scale(float x, float y);
	void scale(const Vector2f& size);

	void move(float x, float y);
	void move(const Vector2f& offset);

	void setRotation(float angle);

	void setSize(float x, float y);
	void setSize(const Vector2f& size);

	void setOrigin(float x, float y);
	void setOrigin(const Vector2f& origin);

	void setScale(float x, float y);
	void setScale(const Vector2f& size);	

	void setPosition(float x, float y);
	void setPosition(const Vector2f& position);

	float getRotation() const;
	Vector2f getSize() const;
	Vector2f getScale() const;
	Vector2f getOrigin() const;
	Vector2f getPosition() const;

	const Matrix& getTransformMatrix();
	const Matrix& getInverseTransformMatrix();

protected:
	virtual void onTransformChangeCallback() {}

	float m_rotation;
	Vector2f m_size;
	Vector2f m_scale;
	Vector2f m_origin;
	Vector2f m_position;

	Matrix m_transformMatrix;
	Matrix m_inverseTransformMatrix;

	bool m_isTransformMatrixUpdateNeeded;
	bool m_isInverseTransformMatrixUpdateNeeded;
};

}
