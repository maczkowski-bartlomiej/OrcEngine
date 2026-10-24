#include "OrcPch.hpp"

#include "Audio/Bus.hpp"

#include <fmod_studio.hpp>
#include <fmod_studio_common.h>

#include "Engine/Debug.hpp"
#include "Engine/Utility.hpp"

namespace orc {

Bus::Bus(FMOD::Studio::Bus* bus)
	: m_bus(bus)
{
	ORC_ASSERT(m_bus, "Attempted to create a null bus.");
}

void Bus::stop()
{
	fmodCall(m_bus->stopAllEvents(FMOD_STUDIO_STOP_IMMEDIATE), "Failed to stop events on a bus.");
}

void Bus::pause()
{
	fmodCall(m_bus->setPaused(true), "Failed to pause a bus.");
}

void Bus::resume()
{
	fmodCall(m_bus->setPaused(false), "Failed to resume a bus.");
}

float Bus::getVolume() const
{
	float volume = 0.0f;
	fmodCall(m_bus->getVolume(&volume), "Failed to get volume from a bus.");
	return volume;
}

void Bus::setVolume(float volume)
{
	fmodCall(m_bus->setVolume(volume), "Failed to set volume of a bus.");
}

bool Bus::isPaused() const
{
	bool isPaused = false;
	fmodCall(m_bus->getPaused(&isPaused), "Failed to check if a bus is paused.");
	return isPaused;
}

}
