#include "OrcPch.hpp"

#include "Audio/SoundHandle.hpp"
#include "Audio/Audio.hpp"

namespace orc {

SoundHandle::SoundHandle(Audio& audio, uint32_t index, uint32_t generation)
	: m_audio(&audio)
	, m_index(index)
	, m_generation(generation)
{
}

bool SoundHandle::isValid() const
{
	return m_audio != nullptr && m_audio->isSoundValid(m_index, m_generation);
}

void SoundHandle::stop(bool immediate)
{
	if (isValid())
	{
		m_audio->stopSound(m_index, m_generation, immediate);
	}
}

void SoundHandle::pause()
{
	if (isValid())
	{
		m_audio->pauseSound(m_index, m_generation);
	}
}

void SoundHandle::resume()
{
	if (isValid())
	{
		m_audio->resumeSound(m_index, m_generation);
	}
}

void SoundHandle::setVolume(float volume)
{
	if (isValid())
	{
		m_audio->setSoundVolume(m_index, m_generation, volume);
	}
}

void SoundHandle::setPitch(float pitch)
{
	if (isValid())
	{
		m_audio->setSoundPitch(m_index, m_generation, pitch);
	}
}

void SoundHandle::setParameter(std::string_view name, float value)
{
	if (isValid())
	{
		m_audio->setSoundParameter(m_index, m_generation, name, value);
	}
}

} // namespace orc
