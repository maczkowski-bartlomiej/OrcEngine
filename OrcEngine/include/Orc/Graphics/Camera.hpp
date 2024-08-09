#pragma once

#include "Engine/Core.hpp"
#include "Graphics/Rect.hpp"

namespace orc {

class Camera
{
public:
	Camera(const FloatRect& viewPort);
	Camera(float left, float top, float right, float bottom);

	void setZoom(float zoom);
	void setRotation(float angle);
	void setPosition(float x, float y);
	void setPosition(const Vector2f& position);
	void setViewportSize(const FloatRect& viewPort);
	void setViewportSize(float left, float top, float right, float bottom);

	void zoom(float zoom);
	void rotate(float angle);
	void move(float x, float y);
	void move(const Vector2f& offset);

<<<<<<< ours
	float getZoom() const;
||||||| ancestor
	void test(float x);

=======
>>>>>>> theirs
	float getRotation() const;
	Vector2f getPosition() const;
	const Matrix4& getViewProjectionMatrix() const;

private:
	void recalculateViewMatrix();

	Matrix4 m_viewMatrix;
	Matrix4 m_projectionMatrix;
	Matrix4 m_viewProjectionMatrix;

	FloatRect m_viewPort;
	Vector2f m_position;
	
	float m_zoom = 1.0f;
	float m_rotation = 0.0f;
};

}
