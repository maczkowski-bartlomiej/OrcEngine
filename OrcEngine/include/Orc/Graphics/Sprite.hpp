#pragma once

#include "Engine/Core.hpp"

#include "Graphics/Rect.hpp"
#include "Graphics/Color.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Animator.hpp"
#include "Graphics/Texturable.hpp"

namespace orc {

class Sprite : public Texturable
{
public:
	Sprite();
	explicit Sprite(Ref<Texture> texture);
	Sprite(Ref<Texture> texture, const Vector2f& position);

	void setColor(const Color& color);

	Color getColor() const;
	FloatRect getLocalRect() const override;

	Animator& getAnimator();
	const Animator& getAnimator() const;

private:
	void updateVerticesPositions() const override;

	Color m_color;
	Animator m_animator;
};

}
