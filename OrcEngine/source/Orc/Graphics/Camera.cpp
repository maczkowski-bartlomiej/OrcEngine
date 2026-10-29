#include "Graphics/Camera.hpp"

#include "Core/Types.hpp"

namespace orc {

Camera::Camera(const FloatRect& viewport)
	: m_viewMatrix(1.0f)
{
	setViewportSize(viewport);
}

Camera::Camera(float left, float top, float width, float height)
	: m_viewMatrix(1.0f)
{
	setViewportSize(left, top, width, height);
}

void Camera::setZoom(float zoom)
{
	m_zoom = zoom;
	recalculateViewMatrix();
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

void Camera::setViewportSize(const FloatRect& viewport)
{
	setViewportSize(viewport.left, viewport.top, viewport.width, viewport.height);
}

void Camera::setViewportSize(float left, float top, float width, float height)
{
	m_viewport = FloatRect(left, top, width, height);
	m_projectionMatrix = glm::ortho(left, left + width, top + height, top, 1.0f, -1.0f);
	recalculateViewMatrix();
}

void Camera::zoom(float zoom)
{
	setZoom(m_zoom + zoom);
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

float Camera::getZoom() const
{
	return m_zoom;
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
	Vector3f center(m_viewport.width / 2.0f, m_viewport.height / 2.0f, 0.0f);
	m_viewMatrix =
		glm::translate(Matrix4(1.0f), center) *
		glm::scale(Matrix4(1.0f), Vector3f(m_zoom, m_zoom, 1.0f)) *
		glm::rotate(Matrix4(1.0f), -m_rotation, Vector3f(0.0f, 0.0f, 1.0f)) *
		glm::translate(Matrix4(1.0f), Vector3f(-m_position, 0.0f)) *
		glm::translate(Matrix4(1.0f), -center);

	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

}
