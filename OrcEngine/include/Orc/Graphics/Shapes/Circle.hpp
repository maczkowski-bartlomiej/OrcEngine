#pragma once

#include "Engine/Core.hpp"

#include "Graphics/Color.hpp"
#include "Graphics/Drawable.hpp"

namespace orc {

class Circle : public Drawable<CircleVertex, 4>
{
public:
	Circle();
	Circle(float radius);
	Circle(const Vector2f& position, float radius);

	void setRadius(float radius);
	void setThickness(float thickness);
	void setBorderThickness(float borderThickness);

	void setFillColor(const Color& fillColor);
	void setBorderColor(const Color& borderColor);

	float getRadius() const;
	float getThickness() const;
	float getBorderThickness() const;

	Color getFillColor() const;
	Color getBorderColor() const;
	FloatRect getLocalRect() const override;
	
private:
	void updateVerticesPositions() const override;

	float m_radius;
	float m_thickness;
	float m_borderThickness;

	Color m_fillColor;
	Color m_borderColor;
};

}
