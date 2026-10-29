#include "Graphics/Animator.hpp"

#include "Graphics/Sprite.hpp"

namespace orc {

Animator::Animator(Sprite* sprite)
	: m_sprite(sprite)
{
	ORC_FATAL_CHECK(m_sprite != nullptr, "Attempted to create Animator with a null Sprite pointer!");
}

void Animator::addAnimation(std::string_view name, Ref<Animation> animation)
{
	m_animations.insert_or_assign(std::string(name), std::move(animation));
}

void Animator::playAnimation(std::string_view name, bool looping)
{
	auto animation = m_animations.find(name);
	ORC_FATAL_CHECK(animation != m_animations.end(), "Animation with name '{}' does not exist!", name);

	m_currentAnimation = animation->second;
	m_currentAnimationLooping = looping;
	m_currentAnimationFrame = 0;
	m_currentAnimationClock.reset();
}

void Animator::setDefaultAnimation(std::string_view name)
{
	auto animation = m_animations.find(name);
	ORC_FATAL_CHECK(animation != m_animations.end(), "Animation with name '{}' does not exist!", name);

	m_defaultAnimation = animation->second;
}

void Animator::update()
{
	if (!m_currentAnimation || m_currentAnimation->frames.empty())
	{
		return;
	}

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
	{
		m_sprite->setTextureRect(m_currentAnimation->frames[m_currentAnimationFrame]);
	}
}

void Animator::setSprite(Sprite* sprite)
{
	ORC_FATAL_CHECK(sprite != nullptr, "Attempted to set a null Sprite pointer on Animator!");
	m_sprite = sprite;
}

}
