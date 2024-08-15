#include "OrcPch.hpp"
#include "Graphics/Camera.hpp"
#include "Engine/Core.hpp"

namespace orc {

Camera::Camera(float left, float right, float bottom, float top) 
	: m_viewMatrix(1.0f), m_projectionMatrix(glm::ortho(left, right, bottom, top, 1.0f, -1.0f))
{
	setViewportSize(left, right, bottom, top);
}

void Camera::setRotation(float angle) 
{
	m_rotation = angle;
	recalculateViewMatrix();
}

void Camera::setPosition(float x, float y) 
{
	m_position = Vector2f(x, y);
	recalculateViewMatrix();
}

void Camera::setPosition(const Vector2f& position) 
{
	m_position = position;
	recalculateViewMatrix();
}

void Camera::setViewportSize(float left, float right, float bottom, float top)
{
	m_projectionMatrix = glm::ortho(left, right, bottom, top, 1.0f, -1.0f);
	recalculateViewMatrix();
}

void Camera::rotate(float angle)
{
	setRotation(m_rotation + angle);
}

void Camera::move(float x, float y)
{
	setPosition(m_position.x + x, m_position.y + y);
}

void Camera::move(const Vector2f& offset)
{
	setPosition(m_position + offset);
}

float Camera::getRotation() const 
{
	return m_rotation;
}

Vector2f Camera::getPosition() const 
{
	return Vector2f(m_position.x, m_position.y);
}

const Matrix4& Camera::getViewProjectionMatrix() const
{
	return m_viewProjectionMatrix;
}

void Camera::recalculateViewMatrix() 
{
	m_viewMatrix = glm::inverse(glm::translate(Matrix4(1.0f), Vector3f(m_position, 0.0f)) *
		glm::rotate(Matrix4(1.0f), m_rotation, Vector3f(0.0f, 0.0f, 1.0f)) *
		glm::scale(Matrix4(1.0f), Vector3f(m_scale, m_scale, 1.0f)));

	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

}
