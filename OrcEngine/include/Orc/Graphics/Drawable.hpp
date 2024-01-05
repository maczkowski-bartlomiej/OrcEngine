#pragma once

#include <array>

#include "Engine/Core.hpp"

#include "Graphics/Rect.hpp"
#include "Graphics/Transformable.hpp"

namespace orc {

template<typename VertexType, uint64_t VerticesCount>
class Drawable : public Transformable
{
public:
	Drawable();

	void setVertices(const std::array<VertexType, VerticesCount>& vertices);

	FloatRect getGlobalRect() const;
	virtual FloatRect getLocalRect() const = 0;

	VertexType getVertex(uint64_t index) const;
	const std::array<VertexType, VerticesCount>& getVertices() const;

protected:
	virtual void updateVerticesPositions() const = 0;

	mutable std::array<VertexType, VerticesCount> m_vertices;

private:
	void onTransformChangeCallback() override;
	
	mutable bool m_isVertexPositionsUpdateNeeded;
};

template<typename VertexType, uint64_t VerticesCount>
Drawable<VertexType, VerticesCount>::Drawable()
	: m_isVertexPositionsUpdateNeeded(true)
{
}



template<typename VertexType, uint64_t VerticesCount>
void Drawable<VertexType, VerticesCount>::setVertices(const std::array<VertexType, VerticesCount>& vertices)
{
	m_vertices = vertices;
}

template<typename VertexType, uint64_t VerticesCount>
FloatRect Drawable<VertexType, VerticesCount>::getGlobalRect() const
{
	FloatRect localRect = getLocalRect();
	const Matrix& transform = getTransformMatrix();

	Vector2f points[] = {
		transform * Vector3f(localRect.x, localRect.y, 1.0f),
		transform * Vector3f(localRect.x, localRect.y + localRect.height, 1.0f),
		transform * Vector3f(localRect.x + localRect.width, localRect.y, 1.0f),
		transform * Vector3f(localRect.x + localRect.width, localRect.y + localRect.height, 1.0f)
	};

	float left = points[0].x;
	float top = points[0].y;
	float right = points[0].x;
	float bottom = points[0].y;

	for (int i = 1; i < 4; ++i)
	{
		if (points[i].x < left)
			left = points[i].x;
		else if (points[i].x > right)
			right = points[i].x;

		if (points[i].y < top)
			top = points[i].y;
		else if (points[i].y > bottom)
			bottom = points[i].y;
	}

	return FloatRect(left, top, right - left, bottom - top);
}

template<typename VertexType, uint64_t VerticesCount>
VertexType Drawable<VertexType, VerticesCount>::getVertex(uint64_t index) const
{
	ORC_ASSERT(index >= 0 && index <= VerticesCount, "Invalid vertex index '{}'", index);

	if (m_isVertexPositionsUpdateNeeded)
	{
		updateVerticesPositions();
		m_isVertexPositionsUpdateNeeded = false;
	}

	return m_vertices[index];
}

template<typename VertexType, uint64_t VerticesCount>
const std::array<VertexType, VerticesCount>& Drawable<VertexType, VerticesCount>::getVertices() const
{
	if (m_isVertexPositionsUpdateNeeded)
	{
		updateVerticesPositions();
		m_isVertexPositionsUpdateNeeded = false;
	}

	return m_vertices;
}

template<typename VertexType, uint64_t VerticesCount>
void Drawable<VertexType, VerticesCount>::onTransformChangeCallback()
{
	m_isVertexPositionsUpdateNeeded = true;
}

}
