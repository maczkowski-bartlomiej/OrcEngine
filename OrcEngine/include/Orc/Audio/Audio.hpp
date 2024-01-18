#pragma once

#include "Engine/Core.hpp"

#include "Audio/SoundBuffer.hpp"
#include "Audio/MusicBuffer.hpp"

#include <AL/alc.h>

#include <list>
#include <vector>

namespace orc {

class Audio
{
public:
	Audio();
	~Audio();

	void update();

	AudioID playSound(Ref<SoundBuffer> soundBuffer, const Vector2f& position, float volume = 1.0f);
	AudioID playMusic(Ref<SoundBuffer> soundBuffer, const Vector2f& position, float volume = 1.0f, bool looping = true);

	void stopSound(AudioID soundID);
	void pauseSound(AudioID soundID);
	void resumeSound(AudioID soundID);

	void pauseMusic();
	void resumeMusic();

	void stopAllSounds();

private:
	void initOpenAL();

	ALCdevice* m_device = nullptr;
	ALCcontext* m_context = nullptr;

	AudioID m_musicID = 0;
	Ref<MusicBuffer> m_musicBuffer;

	std::list<AudioID> m_soundQueue;
	std::vector<AudioID> m_deleteSoundQueue;

	static Audio* m_instance;
};

}
