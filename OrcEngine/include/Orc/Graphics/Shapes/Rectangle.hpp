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
	void setThickness(float thickness);
	void setFillColor(const Color& fillColor);
	void setBorderColor(const Color& borderColor);

	Vector2f getSize() const;
	float getThickness() const;
	Color getFillColor() const;
	Color getBorderColor() const;
	FloatRect getLocalRect() const override;
	
private:
	void updateVerticesPositions() const override;

	Vector2f m_size;
	Color m_fillColor;
	Color m_borderColor;
	float m_thickness;
};

}
