#pragma once

#include <string>
#include <unordered_map>

#include "Engine/Clock.hpp"
#include "Engine/Utility.hpp"
#include "Graphics/Rect.hpp"

namespace orc {

class Sprite;

struct Animation
{
	Animation() = default;
	Animation(const std::vector<FloatRect>& frames, uint32_t durationMs)
		: frames(frames), durationMs(durationMs)
	{
	}

	std::vector<FloatRect> frames;
	uint32_t durationMs = 0u;
};

class Animator
{
public:
	explicit Animator(Sprite* sprite);

	void addAnimation(std::string_view name, Ref<Animation> animation);
	void playAnimation(std::string_view name, bool looping = false);
	void setDefaultAnimation(std::string_view name);

	void update();
	void setSprite(Sprite* sprite);

private:
	Sprite* m_sprite = nullptr;
	Ref<Animation> m_currentAnimation;
	Ref<Animation> m_defaultAnimation;

	Clock m_currentAnimationClock;
	uint64_t m_currentAnimationFrame = 0;
	bool m_currentAnimationLooping = false;

	std::unordered_map<std::string, Ref<Animation>, StringViewHash, std::equal_to<>> m_animations;
};

}
