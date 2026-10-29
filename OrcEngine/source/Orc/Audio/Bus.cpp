#include "Audio/Bus.hpp"

#include <fmod_studio.hpp>
#include <fmod_studio_common.h>

#include "Audio/AudioUtility.hpp"
#include "Core/Debug.hpp"

namespace orc {

Bus::Bus(FMOD::Studio::Bus* bus)
	: m_bus(bus)
{
	ORC_FATAL_CHECK(m_bus != nullptr, "Attempted to create a null bus.");
}

void Bus::stop()
{
	ORC_FMOD_CALL(
		m_bus->stopAllEvents(FMOD_STUDIO_STOP_IMMEDIATE),
		"Failed to stop events on a bus.");
}

void Bus::pause()
{
	ORC_FMOD_CALL(
		m_bus->setPaused(true),
		"Failed to pause a bus.");
}

void Bus::resume()
{
	ORC_FMOD_CALL(
		m_bus->setPaused(false),
		"Failed to resume a bus.");
}

float Bus::getVolume() const
{
	float volume = 0.0f;
	ORC_FMOD_CALL(
		m_bus->getVolume(&volume),
		"Failed to get volume from a bus.");
	return volume;
}

void Bus::setVolume(float volume)
{
	ORC_FMOD_CALL(
		m_bus->setVolume(volume),
		"Failed to set volume of a bus.");
}

bool Bus::isPaused() const
{
	bool isPaused = false;
	ORC_FMOD_CALL(
		m_bus->getPaused(&isPaused),
		"Failed to check if a bus is paused.");
	return isPaused;
}

}
