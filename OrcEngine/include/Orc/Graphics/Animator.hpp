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
	uint32_t durationMs;
};

class Animator
{
public:
	Animator(Sprite* sprite);

	void addAnimation(const std::string& name, const std::vector<FloatRect>& frames, uint32_t durationMs);
	void playAnimation(const std::string& name, bool looping = false);
	void setDefaultAnimation(const std::string& name);

	void update();

private:
	Sprite* m_sprite = nullptr;
	Animation* m_currentAnimation = nullptr;
	Animation* m_defaultAnimation = nullptr;

	Clock m_currentAnimationClock;
	uint64_t m_currentAnimationFrame = 0;
	bool m_currentAnimationLooping = false;

	
	std::unordered_map<std::string, Animation> m_animations;
};

}
