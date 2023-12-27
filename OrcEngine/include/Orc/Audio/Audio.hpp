#pragma once

#include "Engine/Core.hpp"
#include "Audio/SoundBuffer.hpp"

#include <list>

#include <AL/alc.h>

namespace orc {

class Audio
{
public:
	Audio();
	~Audio();

	void update();

	//AudioID playMusic()
	AudioID playSound(Ref<SoundBuffer> soundBuffer, const Vector2f& position, float volume = 1.0f);

	void pauseSound(AudioID soundID);
	void resumeSound(AudioID soundID);

	void stopSound(AudioID soundID);
	void stopAllSounds();

private:
	void initOpenAL();

	static Audio* m_instance;

	ALCdevice* m_device;
	ALCcontext* m_context;

	std::list<AudioID> m_soundQueue;
	std::vector<AudioID> m_deleteSoundQueue;
};

}
