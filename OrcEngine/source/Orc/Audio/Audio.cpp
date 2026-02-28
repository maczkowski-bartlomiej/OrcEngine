#include "OrcPch.hpp"
#include "Audio/Audio.hpp"
#include "Audio/FmodCall.hpp"
#include "Engine/Debug.hpp"


#include <fmod.hpp>
#include <fmod_common.h>
#include <fmod_studio.hpp>
#include <fmod_studio_common.h>

#include <vector>

namespace orc {

bool Audio::init(const AudioSettings& audioSettings, const std::vector<std::string>& audioBanks)
{
	ORC_LOG_INFO("Initializing audio...");
	if (!FMOD_CALL(FMOD::Studio::System::create(&m_system))) return false;
	if (!FMOD_CALL(m_system->initialize(audioSettings.maxChannels, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0))) return false;

	for (const auto& audioBank : audioBanks)
	{
		if (!loadBank(audioBank))
			return false;
	}

	FMOD::Studio::Bus* bus = nullptr;
	if (!FMOD_CALL(m_system->getBus("bus:/", &bus)))
	{
		ORC_LOG_ERROR("Failed to load master bus");
		return false;
	}
	m_masterBus.setBus(bus);

	if (!FMOD_CALL(m_system->getBus(audioSettings.musicBusName.c_str(), &bus)))
	{
		ORC_LOG_ERROR("Failed to load music bus '{}'", audioSettings.musicBusName);
		return false;
	}
	m_musicBus.setBus(bus);

	if (!FMOD_CALL(m_system->getBus(audioSettings.sfxBusName.c_str(), &bus)))
	{
		ORC_LOG_ERROR("Failed to load SFX bus '{}'", audioSettings.sfxBusName);
		return false;
	}
	m_sfxBus.setBus(bus);

	return true;
}

void Audio::deinit()
{
	ORC_LOG_INFO("Deinitializing audio...");

	if (!FMOD_CALL(m_system->release()))
		ORC_LOG_ERROR("Failed to deinitialize audio...");
}

bool Audio::loadBank(const FilePath& filePath)
{
	FMOD::Studio::Bank* bank = nullptr;
	if (!FMOD_CALL(m_system->loadBankFile(filePath.string().c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank)))
	{
		ORC_LOG_ERROR("Failed to load audio bank '{}'", filePath.string().c_str());
		return false;
	}

	if (!FMOD_CALL(bank->loadSampleData()))
	{
		return false;
	}

	return true;
}

void Audio::play(const std::string& eventPath)
{
	FMOD::Studio::EventDescription* eventDescription = nullptr;
	if (!FMOD_CALL(m_system->getEvent(eventPath.c_str(), &eventDescription)))
	{ 
		ORC_LOG_ERROR("Failed to play audio event '{}'", eventPath);
		return;
	}

	FMOD::Studio::EventInstance* eventInstance = nullptr;
	FMOD_CALL(eventDescription->createInstance(&eventInstance));
	FMOD_CALL(eventInstance->start());
	FMOD_CALL(eventInstance->release());
}

void Audio::update()
{
	m_system->update();
}

Bus& Audio::getSfxBus()
{
	return m_sfxBus;
}

Bus& Audio::getMusicBus()
{
	return m_musicBus;
}

Bus& Audio::getMasterBus()
{
	return m_masterBus;
}

}
