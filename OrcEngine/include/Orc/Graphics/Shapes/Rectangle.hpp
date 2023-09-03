#pragma once

#include "Engine/Core.hpp"

#include "Graphics/Color.hpp"
#include "Graphics/Drawable.hpp"

namespace orc {

class Rectangle : public Drawable<Vertex, 4>
{
public:
	Rectangle();
	Rectangle(const Vector2f& position, const Vector2f& size, Color color, bool fillColor = true);

	void setSize(const Vector2f& size);

	bool isFilled();
	Vector2f getSize() const;
	FloatRect getLocalRect() const override;
	
private:
	void updateVerticesPositions() const override;

	bool m_isFilled;
	Vector2f m_size;
};

}
