#include "OrcPch.hpp"

#include "Graphics/Transform.hpp"

namespace orc {

void Transform::rotate(float angle)
{
	setRotation(m_rotation + angle);
}

void Transform::scale(float x, float y)
{
	setScale(m_scale + Vector2f(x, y));
}

void Transform::scale(const Vector2f& size)
{
	setScale(m_scale + size);
}

void Transform::move(float x, float y)
{
	setPosition(m_position + Vector2f(x, y));
}

void Transform::move(const Vector2f& offset)
{
	setPosition(m_position + offset);
}

void Transform::setRotation(float angle)
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

	m_isMatrixUpdateNeeded = true;
	m_isInverseMatrixUpdateNeeded = true;
}

void Transform::setScale(float x, float y)
{
	m_scale = Vector2f(x, y);

	m_isMatrixUpdateNeeded = true;
	m_isInverseMatrixUpdateNeeded = true;
}

void Transform::setScale(const Vector2f& size)
{
	m_scale = size;

	m_isMatrixUpdateNeeded = true;
	m_isInverseMatrixUpdateNeeded = true;
}

void Transform::setPosition(float x, float y)
{
	m_position = Vector2f(x, y);

	m_isMatrixUpdateNeeded = true;
	m_isInverseMatrixUpdateNeeded = true;
}

void Transform::setPosition(const Vector2f& offset)
{
	m_position = offset;

	m_isMatrixUpdateNeeded = true;
	m_isInverseMatrixUpdateNeeded = true;
}

float Transform::getRotation() const
{
	return m_rotation;
}

Vector2f Transform::getScale() const
{
	return m_scale;
}

Vector2f Transform::getPosition() const
{
	return m_position;
}

const Matrix& Transform::getMatrix()
{
	if (m_isMatrixUpdateNeeded)
	{
		m_transformMatrix = glm::translate(Matrix(1.0f), Vector3f(m_position, 1.0f)) *
							glm::rotate(Matrix(1.0f), glm::radians(m_rotation), Vector3f(0.0f, 0.0f, 1.0f)) *
							glm::scale(Matrix(1.0f), Vector3f(m_scale, 1.0f));
	}

	return m_transformMatrix;
}

const Matrix& Transform::getInverseMatrix()
{
	if (m_isInverseMatrixUpdateNeeded)
	{
		m_inverseTransformMatrix = glm::inverse(getMatrix());
	}

	return m_inverseTransformMatrix;
}

//TransformMatrix TransformMatrix::inverse()
//{
//	return glm::inverse(m_matrix);
//}
//
//TransformMatrix& TransformMatrix::rotate(float angle)
//{
//	m_matrix = glm::rotate(m_matrix, angle, Vector3f(0.0f, 0.0f, 1.0f));
//	return *this;
//}
//
//TransformMatrix& TransformMatrix::scale(float x, float y)
//{
//	m_matrix = glm::scale(m_matrix, Vector3f(x, y, 1.0f));
//	return *this;
//}
//
//TransformMatrix& TransformMatrix::scale(const Vector2f& size)
//{
//	m_matrix = glm::scale(m_matrix, Vector3f(size, 1.0f));
//	return *this;
//}
//
//TransformMatrix& TransformMatrix::translate(float x, float y)
//{
//	m_matrix = glm::translate(m_matrix, Vector3f(x, y, 1.0f));
//	return *this;
//}
//
//TransformMatrix& TransformMatrix::translate(const Vector2f& offset)
//{
//	m_matrix = glm::translate(m_matrix, Vector3f(offset, 1.0f));
//	return *this;
//}


}
