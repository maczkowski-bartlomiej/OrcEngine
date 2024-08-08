#pragma once

#include "Engine/Core.hpp"

#include "Graphics/Rect.hpp"
#include "Graphics/Color.hpp"
#include "Graphics/Vertex.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Texturable.hpp"

namespace orc {

class Circle : public Texturable<CircleVertex, 4>
{
public:
	Circle();
	Circle(float radius, const Color& color = Color());
	Circle(const Vector2f& position, float radius, const Color& color = Color());

	void setFillColor(const Color& fillColor);
	void setBorderColor(const Color& borderColor);

	void setRadius(float radius);
	void setInnerRadius(float innerRadius);
	void setBorderThickness(float borderThickness);

	Color getFillColor() const;
	Color getBorderColor() const;

	float getRadius() const;
	float getInnerRadius() const;
	float getBorderThickness() const;

	FloatRect getLocalRect() const override;
	
private:
	void updateVerticesPositions() const override;

	float m_radius = 0.0f;
	float m_innerRadius = 0.0f;
	float m_borderThickness = 0.0f;

	Color m_fillColor;
	Color m_borderColor;
};

}
