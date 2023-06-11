#pragma once

#include "Engine/Core.hpp"

namespace orc {

class Camera
{
public:
	Camera(float left, float right, float bottom, float top);

	void move(const Vector2f& offset);

	void setRotation(float rotation);
	void setPosition(float x, float y);
	void setPosition(const Vector2f& position);

	float getRotation() const;
	Vector2f getPosition() const;
	const Matrix& getViewProjectionMatrix() const;

private:
	void recalculateViewMatrix();

	float m_rotation;
	Vector2f m_position;

	Matrix m_viewMatrix;
	Matrix m_projectionMatrix;
	Matrix m_viewProjectionMatrix;
};

}
