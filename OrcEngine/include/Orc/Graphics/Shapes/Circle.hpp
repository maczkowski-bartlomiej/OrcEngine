#pragma once

#include "Engine/Core.hpp"

#include "Graphics/Rect.hpp"
#include "Graphics/Color.hpp"
#include "Graphics/Vertex.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Drawable.hpp"

namespace orc {

class Circle : public Drawable<CircleVertex, 4>
{
public:
	Circle();
	Circle(float radius);
	Circle(const Vector2f& position, float radius);

	void setFillColor(const Color& fillColor);
	void setBorderColor(const Color& borderColor);

	void setRadius(float radius);
	void setInnerRadius(float innerRadius);
	void setBorderThickness(float borderThickness);

	void setTexture(Ref<Texture> texture);
	void setTextureRect(const FloatRect& textureRect);

	Color getFillColor() const;
	Color getBorderColor() const;

	float getRadius() const;
	float getInnerRadius() const;
	float getBorderThickness() const;

	Ref<Texture> getTexture() const;
	FloatRect getTextureRect() const;
	FloatRect getLocalRect() const override;
	
private:
	void updateVerticesTexCoords() const;
	void updateVerticesPositions() const override;

	float m_radius;
	float m_innerRadius;
	float m_borderThickness;

	Color m_fillColor;
	Color m_borderColor;

	Ref<Texture> m_texture;
	FloatRect m_textureRect;
};

}
