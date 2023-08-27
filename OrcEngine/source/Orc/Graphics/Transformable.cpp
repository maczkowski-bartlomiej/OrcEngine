#include "OrcPch.hpp"

#include "Graphics/Transformable.hpp"

namespace orc {

Transformable::Transformable()
	: m_scale(1.0f, 1.0f), m_rotation(0.0f), m_isTransformMatrixUpdateNeeded(false), m_isInverseTransformMatrixUpdateNeeded(false)
{
}

void Transformable::rotate(float angle)
{
	setRotation(m_rotation + angle);
}

void Transformable::scale(float x, float y)
{
	setScale(m_scale + Vector2f(x, y));
}

void Transformable::scale(const Vector2f& size)
{
	setScale(m_scale + size);
}

void Transformable::move(float x, float y)
{
	setPosition(m_position + Vector2f(x, y));
}

void Transformable::move(const Vector2f& offset)
{
	setPosition(m_position + offset);
}

void Transformable::setRotation(float angle)
{
	m_rotation = angle;
	
	if (m_rotation > 0.0f)
	{
		while (m_rotation > 360.0f)
			m_rotation -= 360.0f;
	}
	else
	{
		while (m_rotation < 0.0f)
			m_rotation += 360.0f;
	}

	m_isTransformMatrixUpdateNeeded = true;
	m_isInverseTransformMatrixUpdateNeeded = true;

	onTransformChangeCallback();
}

void Transformable::setSize(float x, float y)
{
	m_size = Vector2f(x, y);

	m_isTransformMatrixUpdateNeeded = true;
	m_isInverseTransformMatrixUpdateNeeded = true;

	onTransformChangeCallback();
}

void Transformable::setSize(const Vector2f& size)
{
	m_size = size;

	m_isTransformMatrixUpdateNeeded = true;
	m_isInverseTransformMatrixUpdateNeeded = true;

	onTransformChangeCallback();
}

void Transformable::setOrigin(float x, float y)
{
	m_origin = Vector2f(x, y);

	m_isTransformMatrixUpdateNeeded = true;
	m_isInverseTransformMatrixUpdateNeeded = true;

	onTransformChangeCallback();
}

void Transformable::setOrigin(const Vector2f& origin)
{
	m_origin = origin;

	m_isTransformMatrixUpdateNeeded = true;
	m_isInverseTransformMatrixUpdateNeeded = true;

	onTransformChangeCallback();
}

void Transformable::setScale(float x, float y)
{
	m_scale = Vector2f(x, y);

	m_isTransformMatrixUpdateNeeded = true;
	m_isInverseTransformMatrixUpdateNeeded = true;

	onTransformChangeCallback();
}

void Transformable::setScale(const Vector2f& size)
{
	m_scale = size;

	m_isTransformMatrixUpdateNeeded = true;
	m_isInverseTransformMatrixUpdateNeeded = true;

	onTransformChangeCallback();
}

void Transformable::setPosition(float x, float y)
{
	m_position = Vector2f(x, y);

	m_isTransformMatrixUpdateNeeded = true;
	m_isInverseTransformMatrixUpdateNeeded = true;

	onTransformChangeCallback();
}

void Transformable::setPosition(const Vector2f& position)
{
	m_position = position;

	m_isTransformMatrixUpdateNeeded = true;
	m_isInverseTransformMatrixUpdateNeeded = true;

	onTransformChangeCallback();
}

float Transformable::getRotation() const
{
	return m_rotation;
}

Vector2f Transformable::getSize() const
{
	return m_size;
}

Vector2f Transformable::getScale() const
{
	return m_scale;
}

Vector2f Transformable::getOrigin() const
{
	return m_origin;
}

Vector2f Transformable::getPosition() const
{
	return m_position;
}

const Matrix& Transformable::getTransformMatrix()
{
	if (m_isTransformMatrixUpdateNeeded)
	{
		m_transformMatrix = glm::mat4(1.0f);
		m_transformMatrix = glm::translate(m_transformMatrix, glm::vec3(m_position, 0.0f));
		m_transformMatrix = glm::rotate(m_transformMatrix, glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		m_transformMatrix = glm::translate(m_transformMatrix, glm::vec3(-m_origin, 0.0f));
		m_transformMatrix = glm::scale(m_transformMatrix, glm::vec3(m_size * m_scale, 1.0f));

		m_isTransformMatrixUpdateNeeded = false;
	}

	return m_transformMatrix;
}

const Matrix& Transformable::getInverseTransformMatrix()
{
	if (m_isInverseTransformMatrixUpdateNeeded)
	{
		m_inverseTransformMatrix = glm::inverse(getTransformMatrix());
		m_isInverseTransformMatrixUpdateNeeded = false;
	}

	return m_inverseTransformMatrix;
}

}
