#include "orcPch.hpp"
#include "Graphics/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace orc {

Camera::Camera(float left, float right, float bottom, float top) 
	: m_rotation(0.0f), m_position(0.0f, 0.0f, 0.0f), m_viewMatrix(1.0f), m_projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_viewProjectionMatrix(m_projectionMatrix * m_viewMatrix)
{
}

void Camera::setRotation(float rotation) 
{
	m_rotation = rotation;
	recalculateViewMatrix();
}

void Camera::setPosition(float x, float y) 
{
	m_position = glm::vec3(x, y, 0.0f);
	recalculateViewMatrix();
}

void Camera::setPosition(const Vector2f& position) 
{
	m_position = glm::vec3(position.x, position.y, 0.0f);
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

const glm::mat4& Camera::getViewProjectionMatrix() const 
{
	return m_viewProjectionMatrix;
}

void Camera::recalculateViewMatrix() 
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	m_viewMatrix = glm::inverse(transform);
	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

}
