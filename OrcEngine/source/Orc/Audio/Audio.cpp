#include "OrcPch.hpp"

#include "Audio/Audio.hpp"
#include "Audio/OpenAL.hpp"

#include "Engine/Debug.hpp"

#include <AL/al.h>

namespace orc {

Audio* Audio::m_instance = nullptr;

Audio::Audio()
	: m_device(nullptr), m_context(nullptr)
{
	if (m_instance)
	{
		ORC_FATAL("Audio instance already exist!!!");
	}
	else
	{
		m_instance = this;
		initOpenAL();
	}
}

Audio::~Audio()
{
	if (m_instance == this)
	{
		stopAllSounds();
		alcCall(alcDestroyContext, m_device, m_context);
		alcCall(alcCloseDevice, m_device, m_device);
	}
}

void Audio::update()
{
	m_soundQueue.remove_if(
		[&](AudioID soundID)
		{
			ALint playing = 0;
			alCall(alGetSourcei, soundID, AL_SOURCE_STATE, &playing);

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

	alCall(alDeleteSources, (ALsizei)m_deleteSoundQueue.size(), m_deleteSoundQueue.data());
	m_deleteSoundQueue.clear();
}

AudioID Audio::playSound(Ref<SoundBuffer> soundBuffer, const Vector2f& position, float volume)
{
	AudioID soundID = 0;
	alCall(alGenSources, 1, &soundID);
	alCall(alSourcef, soundID, AL_GAIN, volume);
	alCall(alSource3f, soundID, AL_POSITION, position.x, position.y, 0.0f);
	alCall(alSourcei, soundID, AL_BUFFER, soundBuffer->getAudioID());

	m_soundQueue.push_back(soundID);
	alCall(alSourcePlay, soundID);

	return soundID;
}

void Audio::pauseSound(AudioID soundID)
{
	if (alCall(alIsSource, soundID))
	{
		alCall(alSourcePause, soundID);
	}
}

void Audio::resumeSound(AudioID soundID)
{
	if (alCall(alIsSource, soundID))
	{
		alCall(alSourcePlay, soundID);
	}
}

void Audio::stopSound(AudioID soundID)
{
	if (alCall(alIsSource, soundID))
	{
		alCall(alSourceStop, soundID);
	}
}

void Audio::stopAllSounds()
{
	for (AudioID soundID : m_soundQueue)
	{
		alCall(alDeleteSources, 1, &soundID);
	}

	m_soundQueue.clear();
	m_deleteSoundQueue.clear();
}

void Audio::initOpenAL()
{
	m_device = alcOpenDevice(nullptr);
	ORC_FATAL_CHECK(m_device, "Failed to get sound device");

	m_context = alcCall(alcCreateContext, m_device, m_device, nullptr);
	ORC_FATAL_CHECK(m_context, "Failed to create sound context");

	ORC_FATAL_CHECK(alcCall(alcMakeContextCurrent, m_device, m_context), "Failed to set sound context");

	int32 alcMajorVersion, alcMinorVersion;
	alcGetIntegerv(m_device, ALC_MAJOR_VERSION, 1, &alcMajorVersion);
	alcGetIntegerv(m_device, ALC_MINOR_VERSION, 1, &alcMinorVersion);

	std::string deviceSpecifier = alcCall(alcGetString, m_device, m_device, ALC_DEVICE_SPECIFIER);
	std::string extensions = alcCall(alcGetString, m_device, m_device, ALC_EXTENSIONS);

	ORC_LOG_INFO("Audio info...\n\tOpenAL version: {}.{}\n\tdevice: {}\n\textensions: {}", alcMajorVersion, alcMinorVersion, deviceSpecifier, extensions);
}

}
