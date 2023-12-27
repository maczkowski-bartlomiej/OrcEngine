#pragma once

#include "Engine/Core.hpp"

#include "Graphics/Color.hpp"
#include "Graphics/Drawable.hpp"

namespace orc {

class Rectangle : public Drawable<RectangleVertex, 4>
{
public:
	Rectangle();
	Rectangle(const Vector2f& size);
	Rectangle(const Vector2f& position, const Vector2f& size);

	void setSize(const Vector2f& size);
	void setBorderThickness(float borderThickness);

	void setFillColor(const Color& fillColor);
	void setBorderColor(const Color& borderColor);

	Vector2f getSize() const;
	float getBorderThickness() const;
	FloatRect getLocalRect() const override;

	Color getFillColor() const;
	Color getBorderColor() const;
	
private:
	void updateVerticesPositions() const override;

	Vector2f m_size;
	float m_borderThickness;

	Color m_fillColor;
	Color m_borderColor;
};

}
