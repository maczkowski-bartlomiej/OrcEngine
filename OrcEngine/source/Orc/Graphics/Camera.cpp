#include "OrcPch.hpp"
#include "Graphics/Camera.hpp"
#include "Engine/Core.hpp"

namespace orc {

Camera::Camera(const FloatRect& viewPort)
	: m_viewMatrix(1.0f)
{
	setViewportSize(viewPort);
}

Camera::Camera(float left, float top, float right, float bottom)
	: m_viewMatrix(1.0f)
{
	setViewportSize(left, top, right, bottom);
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

void Camera::setViewportSize(const FloatRect& viewPort)
{
	setViewportSize(viewPort.left, viewPort.top, viewPort.right, viewPort.bottom);
}

void Camera::setViewportSize(float left, float top, float right, float bottom)
{
	m_viewPort = FloatRect(left, top, right, bottom);
	m_projectionMatrix = glm::ortho(left, right, bottom, top, 1.0f, -1.0f);
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
	Vector3f center((m_viewPort.right - m_viewPort.left) / 2.0f, (m_viewPort.bottom - m_viewPort.top) / 2.0f, 0.0f);
	m_viewMatrix =
		glm::translate(Matrix4(1.0f), center) * 
		glm::scale(Matrix4(1.0f), Vector3f(m_zoom, m_zoom, 1.0f)) *
		glm::rotate(Matrix4(1.0f), -m_rotation, Vector3f(0.0f, 0.0f, 1.0f)) *
		glm::translate(Matrix4(1.0f), Vector3f(-m_position, 0.0f)) *
		glm::translate(Matrix4(1.0f), -center);

	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

}
