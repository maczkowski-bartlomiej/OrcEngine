#pragma once

#include "Engine/Core.hpp"

namespace orc {

class Camera
{
public:
	Camera(float left, float right, float bottom, float top);

	void setRotation(float angle);
	void setPosition(float x, float y);
	void setPosition(const Vector2f& position);
	void setViewportSize(float left, float right, float bottom, float top);

	void rotate(float angle);
	void move(float x, float y);
	void move(const Vector2f& offset);

	void test(float x);

	float getRotation() const;
	Vector2f getPosition() const;
	const Matrix4& getViewProjectionMatrix() const;

private:
	void recalculateViewMatrix();

	float m_scale = 1.0f;
	float m_rotation = 0.0f;
	Vector2f m_position;

	Matrix4 m_viewMatrix;
	Matrix4 m_projectionMatrix;
	Matrix4 m_viewProjectionMatrix;
};

}
