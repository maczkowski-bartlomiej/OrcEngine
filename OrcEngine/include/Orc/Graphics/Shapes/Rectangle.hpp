#pragma once

#include "Engine/Core.hpp"

#include "Graphics/Rect.hpp"
#include "Graphics/Color.hpp"
#include "Graphics/Vertex.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Texturable.hpp"

namespace orc {

class Rectangle : public Texturable<RectangleVertex, 4>
{
public:
	Rectangle() = default;
	Rectangle(const Vector2f& size, const Color& color = Color());
	Rectangle(const Vector2f& position, const Vector2f& size, const Color& color = Color());

	void setFillColor(const Color& fillColor);
	void setBorderColor(const Color& borderColor);

	void setSize(const Vector2f& size);
	void setBorderThickness(float borderThickness);

	Color getFillColor() const;
	Color getBorderColor() const;

	Vector2f getSize() const;
	float getBorderThickness() const;

	FloatRect getLocalRect() const override;
	
private:
	void updateVerticesPositions() const override;

	float m_borderThickness = 0.0f;

	Color m_fillColor;
	Color m_borderColor;

	Vector2f m_size;
};

}
