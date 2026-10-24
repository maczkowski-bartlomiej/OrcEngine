#pragma once

#include <fmod_studio.hpp>

namespace orc {

class Bus
{
public:
	explicit Bus(FMOD::Studio::Bus* bus);

	void stop();
	void pause();
	void resume();

	float getVolume() const;
	void setVolume(float volume);

	bool isPaused() const;

private:
	FMOD::Studio::Bus* m_bus = nullptr;
};

}
