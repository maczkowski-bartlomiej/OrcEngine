#include "OrcPch.hpp"

#include "Audio/Bus.hpp"

#include <fmod_studio.hpp>
#include <fmod_studio_common.h>

#include "AudioUtility.hpp"
#include "Engine/Debug.hpp"

namespace orc {

Bus::Bus(void* nativeBus)
	: m_nativeBus(nativeBus)
{
}

bool Bus::isValid() const
{
	return m_nativeBus != nullptr;
}

void Bus::stop()
{
	ORC_ASSERT(m_nativeBus, "Attempted to stop events on a null bus.");
	auto* bus = reinterpret_cast<FMOD::Studio::Bus*>(m_nativeBus);
	if (!fmodCall(bus->stopAllEvents(FMOD_STUDIO_STOP_IMMEDIATE)))
	{
		ORC_LOG_ERROR("Failed to stop events on a bus.");
	}
}

void Bus::pause()
{
	ORC_ASSERT(m_nativeBus, "Attempted to pause a null bus.");
	auto* bus = reinterpret_cast<FMOD::Studio::Bus*>(m_nativeBus);
	if (!fmodCall(bus->setPaused(true)))
	{
		ORC_LOG_ERROR("Failed to pause a bus.");
	}
}

void Bus::resume()
{
	ORC_ASSERT(m_nativeBus, "Attempted to resume a null bus.");
	auto* bus = reinterpret_cast<FMOD::Studio::Bus*>(m_nativeBus);
	if (!fmodCall(bus->setPaused(false)))
	{
		ORC_LOG_ERROR("Failed to resume a bus.");
	}
}

float Bus::getVolume() const
{
	ORC_ASSERT(m_nativeBus, "Attempted to get volume from a null bus.");
	const auto* bus = reinterpret_cast<const FMOD::Studio::Bus*>(m_nativeBus);

	float volume = 0.0f;
	if (!fmodCall(bus->getVolume(&volume)))
	{
		ORC_LOG_ERROR("Failed to get volume from a bus.");
	}
	return volume;
}

void Bus::setVolume(float volume)
{
	ORC_ASSERT(m_nativeBus, "Attempted to set volume of a null bus.");
	auto* bus = reinterpret_cast<FMOD::Studio::Bus*>(m_nativeBus);
	if (!fmodCall(bus->setVolume(volume)))
	{
		ORC_LOG_ERROR("Failed to set volume of a bus.");
	}
}

bool Bus::isPaused() const
{
	ORC_ASSERT(m_nativeBus, "Attempted to check if a null bus is paused.");
	const auto* bus = reinterpret_cast<const FMOD::Studio::Bus*>(m_nativeBus);
	bool isPaused = false;
	if (!fmodCall(bus->getPaused(&isPaused)))
	{
		ORC_LOG_ERROR("Failed to check if a bus is paused.");
	}
	return isPaused;
}

} // namespace orc
