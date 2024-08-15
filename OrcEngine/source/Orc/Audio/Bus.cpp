#include "OrcPch.hpp"
#include "Audio/Bus.hpp"
#include "Audio/FmodCall.hpp"

#include <fmod.hpp>
#include <fmod_common.h>
#include <fmod_studio.hpp>
#include <fmod_studio_common.h>

namespace orc {

Bus::Bus(FMOD::Studio::Bus* bus)
	: m_bus(bus)
{
}

void Bus::setBus(FMOD::Studio::Bus* bus)
{
	m_bus = bus;
}

void Bus::stop()
{
	FMOD_CALL(m_bus->stopAllEvents(FMOD_STUDIO_STOP_IMMEDIATE));
}

void Bus::pause()
{
	FMOD_CALL(m_bus->setPaused(true));
}

void Bus::resume()
{
	FMOD_CALL(m_bus->setPaused(false));
}

float Bus::getVolume() const
{
	float volume = 0.0f;
	FMOD_CALL(m_bus->getVolume(&volume));
	return volume;
}

void Bus::setVolume(float volume)
{
	FMOD_CALL(m_bus->setVolume(volume));
}

bool Bus::isPaused() const
{
	bool isPaused = false;
	FMOD_CALL(m_bus->getPaused(&isPaused));
	return isPaused;
}

}
