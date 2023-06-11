#include "OrcPch.hpp"

#include "Graphics/Camera.hpp"

namespace orc {

Camera::Camera(float left, float right, float bottom, float top) 
	: m_rotation(0.0f), m_position(0.0f, 0.0f), m_viewMatrix(1.0f), m_projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_viewProjectionMatrix(m_projectionMatrix * m_viewMatrix)
{
}

void Camera::move(const Vector2f& offset)
{
	setPosition(m_position.x + offset.x, m_position.y + offset.y);
}

void Camera::setRotation(float rotation) 
{
	m_rotation = rotation;
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

float Camera::getRotation() const 
{
	return m_rotation;
}

Vector2f Camera::getPosition() const 
{
	return Vector2f(m_position.x, m_position.y);
}

const Matrix& Camera::getViewProjectionMatrix() const
{
	return m_viewProjectionMatrix;
}

void Camera::recalculateViewMatrix() 
{
	Matrix transform = glm::translate(Matrix(1.0f), Vector3f(m_position, 1.0f)) * glm::rotate(Matrix(1.0f), glm::radians(m_rotation), Vector3f(0.0f, 0.0f, 1.0f));
	m_viewMatrix = glm::inverse(transform);
	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

}
