#pragma once

#include "Engine/Core.hpp"

#include "Graphics/Rect.hpp"
#include "Graphics/Color.hpp"
#include "Graphics/Vertex.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Drawable.hpp"

namespace orc {

class Rectangle : public Drawable<RectangleVertex, 4>
{
public:
	Rectangle();
	Rectangle(const Vector2f& size);
	Rectangle(const Vector2f& position, const Vector2f& size);

	void setFillColor(const Color& fillColor);
	void setBorderColor(const Color& borderColor);

	void setSize(const Vector2f& size);
	void setBorderThickness(float borderThickness);

	void setTexture(Ref<Texture> texture);
	void setTextureRect(const FloatRect& textureRect);

	Color getFillColor() const;
	Color getBorderColor() const;

	Vector2f getSize() const;
	float getBorderThickness() const;

	Ref<Texture> getTexture() const;
	FloatRect getTextureRect() const;
	FloatRect getLocalRect() const override;
	
private:
	void updateVerticesTexCoords() const;
	void updateVerticesPositions() const override;

	float m_borderThickness;

	Color m_fillColor;
	Color m_borderColor;

	Vector2f m_size;
	Ref<Texture> m_texture;
	FloatRect m_textureRect;
};

}
