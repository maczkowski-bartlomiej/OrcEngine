#include "OrcPch.hpp"

#include "Graphics/Sprite.hpp"
#include "Graphics/Animator.hpp"

namespace orc {

Animator::Animator(Sprite* sprite)
	: m_sprite(sprite)
{
}

void Animator::addAnimation(const std::string& name, const std::vector<FloatRect>& frames, uint32_t durationMs)
{
	m_animations[name] = Animation(frames, durationMs);
}

void Animator::playAnimation(const std::string& name, bool looping)
{
	auto animation = m_animations.find(name);
	if (animation == m_animations.end())
	{
		ORC_LOG_ERROR("Animation with name '", name, "' does not exist!");
		return;
	}

	m_currentAnimation = &animation->second;
	m_currentAnimationLooping = looping;
	m_currentAnimationFrame = 0;
	m_currentAnimationClock.reset();
}

void Animator::setDefaultAnimation(const std::string& name)
{
	auto animation = m_animations.find(name);
	if (animation == m_animations.end())
	{
		ORC_LOG_ERROR("Animation with name '", name, "' does not exist!");
		return;
	}

	m_defaultAnimation = &animation->second;
}

void Animator::update()
{
	if (!m_currentAnimation)
		return;

	if (m_currentAnimationClock.elapsedMs() >= m_currentAnimation->durationMs / m_currentAnimation->frames.size())
	{
		m_currentAnimationClock.reset();
		m_currentAnimationFrame++;

		if (m_currentAnimationFrame >= m_currentAnimation->frames.size())
		{
			m_currentAnimationFrame = 0;
			if (!m_currentAnimationLooping)
			{
				if (m_defaultAnimation)
				{
					m_currentAnimation = m_defaultAnimation;
				}
				else
				{
					m_currentAnimation = nullptr;
				}
			}
		}
	}

	if (m_currentAnimation)
		m_sprite->setTextureRect(m_currentAnimation->frames[m_currentAnimationFrame]);
}

}
