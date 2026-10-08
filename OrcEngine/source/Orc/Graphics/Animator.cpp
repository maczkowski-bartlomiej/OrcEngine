#include "OrcPch.hpp"

#include "Graphics/Sprite.hpp"
#include "Graphics/Animator.hpp"

namespace orc {

Animator::Animator(Sprite* sprite)
	: m_sprite(sprite)
{
}

void Animator::addAnimation(const std::string& name, Ref<Animation> animation)
{
	m_animations[name] = animation;
}

void Animator::playAnimation(const std::string& name, bool looping)
{
	auto animation = m_animations.find(name);
	if (animation == m_animations.end())
	{
		ORC_LOG_ERROR("Animation with name '{}' does not exist!", name);
		return;
	}

	m_currentAnimation = animation->second;
	m_currentAnimationLooping = looping;
	m_currentAnimationFrame = 0;
	m_currentAnimationClock.reset();
}

void Animator::setDefaultAnimation(const std::string& name)
{
	auto animation = m_animations.find(name);
	if (animation == m_animations.end())
	{
		ORC_LOG_ERROR("Animation with name '{}' does not exist!", name);
		return;
	}

	m_defaultAnimation = animation->second;
}

void Animator::update()
{
	if (!m_currentAnimation || m_currentAnimation->frames.empty())
		return;

	const uint64_t frameCount = m_currentAnimation->frames.size();
	const uint32_t frameDuration = frameCount > 0 ? (m_currentAnimation->durationMs / static_cast<uint32_t>(frameCount)) : 0;

	if (frameDuration > 0 && m_currentAnimationClock.elapsedMs() >= frameDuration)
	{
		m_currentAnimationClock.reset();
		m_currentAnimationFrame++;

		if (m_currentAnimationFrame >= frameCount)
		{
			m_currentAnimationFrame = 0;
			if (!m_currentAnimationLooping)
			{
				m_currentAnimation = m_defaultAnimation;
			}
		}
	}

	if (m_currentAnimation && m_currentAnimationFrame < m_currentAnimation->frames.size())
		m_sprite->setTextureRect(m_currentAnimation->frames[m_currentAnimationFrame]);
}

}
