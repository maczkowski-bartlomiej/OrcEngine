#pragma once

#include <array>

#include "Engine/Core.hpp"

#include "Graphics/Rect.hpp"
#include "Graphics/Color.hpp"
#include "Graphics/Vertex.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Transformable.hpp"

namespace orc {

template<typename VertexType, uint64 VerticesCount>
class Drawable : public Transformable
{
public:
	Drawable();
	Drawable(Ref<Texture> texture);

	void setTexture(Ref<Texture> texture);
	void setTextureRect(const FloatRect& textureRect);
	void setVertices(const std::array<VertexType, VerticesCount>& vertices);

	Ref<Texture> getTexture() const;
	FloatRect getGlobalRect() const;
	FloatRect getTextureRect() const;

	VertexType getVertex(uint64 index) const;
	const std::array<VertexType, VerticesCount>& getVertices() const;

	virtual FloatRect getLocalRect() const = 0;

protected:
	virtual void updateVerticesPositions() const = 0;

	Color m_color;
	FloatRect m_textureRect;
	Ref<Texture> m_texture;
	mutable std::array<VertexType, VerticesCount> m_vertices;

private:
	void updateVertexTextureCoords();
	void onTransformChangeCallback() override;
	
	mutable bool m_isVertexPositionsUpdateNeeded;
};

template<typename VertexType, uint64 VerticesCount>
Drawable<VertexType, VerticesCount>::Drawable()
	: m_isVertexPositionsUpdateNeeded(true)
{
	updateVertexTextureCoords();
}

template<typename VertexType, uint64 VerticesCount>
Drawable<VertexType, VerticesCount>::Drawable(Ref<Texture> texture)
	: m_isVertexPositionsUpdateNeeded(true)
{
	setTexture(texture);
}

template<typename VertexType, uint64 VerticesCount>
void Drawable<VertexType, VerticesCount>::setTexture(Ref<Texture> texture)
{
	m_texture = texture;
	setTextureRect(FloatRect(0.0f, 0.0f, texture->getSize().x, texture->getSize().y));
}

template<typename VertexType, uint64 VerticesCount>
void Drawable<VertexType, VerticesCount>::setTextureRect(const FloatRect& textureRect)
{
	m_textureRect = textureRect;
	updateVertexTextureCoords();
}

template<typename VertexType, uint64 VerticesCount>
void Drawable<VertexType, VerticesCount>::setVertices(const std::array<VertexType, VerticesCount>& vertices)
{
	m_vertices = vertices;
}

template<typename VertexType, uint64 VerticesCount>
Ref<Texture> Drawable<VertexType, VerticesCount>::getTexture() const
{
	return m_texture;
}

template<typename VertexType, uint64 VerticesCount>
FloatRect Drawable<VertexType, VerticesCount>::getTextureRect() const
{
	return m_textureRect;
}

template<typename VertexType, uint64 VerticesCount>
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

template<typename VertexType, uint64 VerticesCount>
VertexType Drawable<VertexType, VerticesCount>::getVertex(uint64 index) const
{
	ORC_ASSERT(index >= 0 && index <= VerticesCount, "Invalid vertex index '{}'", index);

	if (m_isVertexPositionsUpdateNeeded)
	{
		updateVerticesPositions();
		m_isVertexPositionsUpdateNeeded = false;
	}

	return m_vertices[index];
}

template<typename VertexType, uint64 VerticesCount>
const std::array<VertexType, VerticesCount>& Drawable<VertexType, VerticesCount>::getVertices() const
{
	if (m_isVertexPositionsUpdateNeeded)
	{
		updateVerticesPositions();
		m_isVertexPositionsUpdateNeeded = false;
	}

	return m_vertices;
}

template<typename VertexType, uint64 VerticesCount>
void Drawable<VertexType, VerticesCount>::updateVertexTextureCoords()
{
	if (m_texture)
	{
		FloatRect clipSpacedTextureRect = m_textureRect;
		clipSpacedTextureRect.x /= m_texture->getSize().x;
		clipSpacedTextureRect.y /= m_texture->getSize().y;
		clipSpacedTextureRect.width /= m_texture->getSize().x;
		clipSpacedTextureRect.height /= m_texture->getSize().y;

		m_vertices[0].textureCoords = Vector2f(clipSpacedTextureRect.x, clipSpacedTextureRect.y);
		m_vertices[1].textureCoords = Vector2f(clipSpacedTextureRect.width, clipSpacedTextureRect.y);
		m_vertices[2].textureCoords = Vector2f(clipSpacedTextureRect.x, clipSpacedTextureRect.height);
		m_vertices[3].textureCoords = Vector2f(clipSpacedTextureRect.width, clipSpacedTextureRect.height);
	}
	else
	{
		m_vertices[0].textureCoords = Vector2f(0.0f, 0.0f);
		m_vertices[1].textureCoords = Vector2f(1.0f, 0.0f);
		m_vertices[2].textureCoords = Vector2f(0.0f, 1.0f);
		m_vertices[3].textureCoords = Vector2f(1.0f, 1.0f);
	}
}

template<typename VertexType, uint64 VerticesCount>
void Drawable<VertexType, VerticesCount>::onTransformChangeCallback()
{
	m_isVertexPositionsUpdateNeeded = true;
}

}
