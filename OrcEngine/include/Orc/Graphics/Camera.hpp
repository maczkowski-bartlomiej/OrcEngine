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
	const Matrix4& getViewProjectionMatrix() const;

private:
	void recalculateViewMatrix();

	float m_scale;
	float m_rotation;
	Vector2f m_position;

	Matrix4 m_viewMatrix;
	Matrix4 m_projectionMatrix;
	Matrix4 m_viewProjectionMatrix;
};

}
