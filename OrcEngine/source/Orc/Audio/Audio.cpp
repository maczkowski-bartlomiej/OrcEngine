#include "OrcPch.hpp"

#include "Audio/Audio.hpp"
#include "Audio/FmodCall.hpp"
#include "Engine/Debug.hpp"

#include <fmod.hpp>
#include <fmod_common.h>
#include <fmod_studio.hpp>
#include <fmod_studio_common.h>

namespace orc {

Audio::Audio(const AudioSettings& audioSettings, const std::vector<std::string>& audioBanks)
{
	FMOD_CALL(FMOD::Studio::System::create(&m_system));
	FMOD_CALL(m_system->initialize(audioSettings.maxChannels, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));

	for (const auto& audioBank : audioBanks)
	{
		if (!loadBank(audioBank)) continue;
	}

	FMOD::Studio::Bus* bus = nullptr;
	FMOD_CALL(m_system->getBus("bus:/", &bus));
	if (!bus) { ORC_LOG_ERROR("Couldn't get master bus"); return; }
	m_masterBus = createUniquePtr<Bus>(bus);

	bus = nullptr;
	FMOD_CALL(m_system->getBus(audioSettings.musicBusName.c_str(), &bus));
	if (!bus) { ORC_LOG_ERROR("Couldn't get bus '{}'", audioSettings.musicBusName); return; }
	m_musicBus = createUniquePtr<Bus>(bus);

	bus = nullptr;
	FMOD_CALL(m_system->getBus(audioSettings.sfxBusName.c_str(), &bus));
	if (!bus) { ORC_LOG_ERROR("Couldn't get bus '{}'", audioSettings.sfxBusName); return; }
	m_sfxBus = createUniquePtr<Bus>(bus);
}

Audio::~Audio()
{
	FMOD_CALL(m_system->release());
}

bool Audio::loadBank(const FilePath& filePath)
{
	FMOD::Studio::Bank* bank = nullptr;
	FMOD_CALL(m_system->loadBankFile(filePath.string().c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank));
	if (!bank) { ORC_LOG_ERROR("Couldn't load bank '{}'", filePath.string().c_str()); return false; };
	FMOD_CALL(bank->loadSampleData());

	return true;
}

void Audio::play(const std::string& eventPath)
{
	FMOD::Studio::EventDescription* eventDescription = nullptr;
	FMOD_CALL(m_system->getEvent(eventPath.c_str(), &eventDescription));
	if (!eventDescription) {  ORC_LOG_ERROR("Couldn't play sound '{}'", eventPath); return; }
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
	return *m_sfxBus;
}

Bus& Audio::getMusicBus()
{
	return *m_musicBus;
}

Bus& Audio::getMasterBus()
{
	return *m_masterBus;
}


}
