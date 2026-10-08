#include "OrcPch.hpp"
#include "Audio/Bus.hpp"
#include "Engine/Debug.hpp"
#include "Engine/Utility.hpp"

#include <fmod_studio.hpp>
#include <fmod_studio_common.h>
#include <string>

namespace orc {

	void Bus::load(FMOD::Studio::System* system, const std::string& busName)
	{
		ORC_ASSERT(system, "Uninitialized audio system.");

		FMOD::Studio::Bus* bus = nullptr; 
		if (!fmodCall(system->getBus(busName.c_str(), &bus)))
		{
			ORC_LOG_ERROR("Failed to load bus '{}'.", busName);
		}
		ORC_ASSERT(bus, "FMOD returned empty bus '{}'.", busName);

		m_bus = bus;
	}

	void Bus::stop()
	{
		ORC_ASSERT(m_bus, "Attempted to stop events on a null bus.");
		if (!fmodCall(m_bus->stopAllEvents(FMOD_STUDIO_STOP_IMMEDIATE)))
		{
			ORC_LOG_ERROR("Failed to stop events on a bus.");
		}
	}

	void Bus::pause()
	{
		ORC_ASSERT(m_bus, "Attempted to pause a null bus.");
        if (!fmodCall(m_bus->setPaused(true)))
		{
			ORC_LOG_ERROR("Failed to pause a bus.");
		}
	}

	void Bus::resume()
	{
		ORC_ASSERT(m_bus, "Attempted to resume a null bus.");
		if (!fmodCall(m_bus->setPaused(false)))
		{
			ORC_LOG_ERROR("Failed to resume a bus.");
		}
	}

	float Bus::getVolume() const
	{
		ORC_ASSERT(m_bus, "Attempted to get volume from a null bus.");

		float volume = 0.0f;
        if (!fmodCall(m_bus->getVolume(&volume)))
		{
			ORC_LOG_ERROR("Failed to get volume from a bus");
		}
		return volume;
	}

	void Bus::setVolume(float volume)
	{
		ORC_ASSERT(m_bus, "Attempted to set volume from a null bus.");
		 if (!fmodCall(m_bus->setVolume(volume)))
		{
			ORC_LOG_ERROR("Failed to set volume of a bus");
		}
	}

	bool Bus::isPaused() const
	{
		ORC_ASSERT(m_bus, "Attempted to check if a null bus is paused.");
		bool isPaused = false;
		if (!fmodCall(m_bus->getPaused(&isPaused)))
		{
			ORC_LOG_ERROR("Failed to check if a bus is paused.");
		}
		return isPaused;
	}

}
