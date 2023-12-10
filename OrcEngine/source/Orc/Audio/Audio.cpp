#include "OrcPch.hpp"

#include "Audio/Audio.hpp"

#include "Engine/Debug.hpp"

#include <AL/al.h>

namespace orc {

Audio::Audio()
{
	m_device = alcOpenDevice(nullptr);
	ORC_FATAL_CHECK(m_device, "Failed to get sound device");

	m_context = alcCreateContext(m_device, nullptr);
	ORC_FATAL_CHECK(m_context, "Failed to create sound context");

	ORC_FATAL_CHECK(alcMakeContextCurrent(m_context), "Failed to set sound context");

	int32 alcMajorVersion, alcMinorVersion;
	alcGetIntegerv(m_device, ALC_MAJOR_VERSION, 1, &alcMajorVersion);
	alcGetIntegerv(m_device, ALC_MINOR_VERSION, 1, &alcMinorVersion);

	std::string deviceSpecifier = alcGetString(m_device, ALC_DEVICE_SPECIFIER);
	std::string extensions = alcGetString(m_device, ALC_EXTENSIONS);

	ORC_LOG_INFO("Audio info...\n\tOpenAL version: {}.{}\n\tdevice: {}\n\textensions: {}", alcMajorVersion, alcMinorVersion, deviceSpecifier, extensions);
}

Audio::~Audio()
{
	stopAllSounds();
	alcDestroyContext(m_context);
	alcCloseDevice(m_device);
}

void Audio::update()
{
	m_soundQueue.remove_if(
		[&](AudioID soundID)
		{
			ALint playing = 0;
			alGetSourcei(soundID, AL_SOURCE_STATE, &playing);

			if (playing == AL_STOPPED)
			{
				m_deleteSoundQueue.push_back(soundID);
				return true;
			}
			else
			{
				return false;
			}
		}
	);

	alDeleteSources((ALsizei)m_deleteSoundQueue.size(), m_deleteSoundQueue.data());
	m_deleteSoundQueue.clear();
}

AudioID Audio::playSound(Ref<SoundBuffer> soundBuffer, const Vector2f& position, float volume)
{
	AudioID soundID = 0;
	alGenSources(1, &soundID);
	alSourcef(soundID, AL_GAIN, volume);
	alSource3f(soundID, AL_POSITION, position.x, position.y, 0.0f);
	alSourcei(soundID, AL_BUFFER, soundBuffer->getAudioID());

	m_soundQueue.push_back(soundID);
	alSourcePlay(soundID);

	ALenum error = alGetError();
	ORC_ERROR_CHECK(!error, "OpenAL error: {}", alGetString(error));

	return soundID;
}

void Audio::pauseSound(AudioID soundID)
{
	if (alIsSource(soundID))
	{
		alSourcePause(soundID);
	}
}

void Audio::resumeSound(AudioID soundID)
{
	if (alIsSource(soundID))
	{
		alSourcePlay(soundID);
	}
}

void Audio::stopSound(AudioID soundID)
{
	if (alIsSource(soundID))
	{
		alSourceStop(soundID);
	}
}

void Audio::stopAllSounds()
{
	for (AudioID soundID : m_soundQueue)
	{
		alDeleteSources(1, &soundID);
	}

	m_soundQueue.clear();
	m_deleteSoundQueue.clear();
}

}
