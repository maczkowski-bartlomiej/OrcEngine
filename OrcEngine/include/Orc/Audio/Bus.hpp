#pragma once

#include <string>

#include <fmod_studio.hpp>

namespace orc {

class Bus
{
public:
	void load(FMOD::Studio::System* system, const std::string& busName);

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
