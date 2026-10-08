#pragma once

#include "Engine/Clock.hpp"
#include "Graphics/Rect.hpp"

#include <string>
#include <unordered_map>

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
	Animator(Sprite* sprite);

	void addAnimation(const std::string& name, Ref<Animation> animation);
	void playAnimation(const std::string& name, bool looping = false);
	void setDefaultAnimation(const std::string& name);

	void update();

private:
	Sprite* m_sprite = nullptr;
	Ref<Animation> m_currentAnimation;
	Ref<Animation> m_defaultAnimation;

	Clock m_currentAnimationClock;
	uint64_t m_currentAnimationFrame = 0;
	bool m_currentAnimationLooping = false;
	
	std::unordered_map<std::string, Ref<Animation>> m_animations;
};

}
