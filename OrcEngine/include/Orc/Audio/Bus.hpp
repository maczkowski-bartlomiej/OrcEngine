#pragma once

#include "Engine/Core.hpp"

namespace FMOD::Studio { class Bus; }

namespace orc {

class Bus
{
public:
	Bus() = default;
	Bus(FMOD::Studio::Bus* bus);

	void setBus(FMOD::Studio::Bus* bus);

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
