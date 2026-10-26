#include "Audio/Audio.hpp"

#include <filesystem>
#include <string>
#include <vector>

#include <fmod_common.h>
#include <fmod_studio.hpp>
#include <fmod_studio_common.h>

#include "Audio/AudioUtility.hpp"
#include "Audio/Bus.hpp"
#include "Core/Core.hpp"
#include "Engine/Debug.hpp"
#include "Engine/Logger.hpp"

namespace orc {

Audio::Audio(const AudioSettings& audioSettings)
{
	ORC_LOG_INFO("Initializing audio.");

	ORC_FMOD_CALL(
		FMOD::Studio::System::create(&m_system),
		"Failed to create FMOD system.");
	ORC_FMOD_CALL(
		m_system->initialize(audioSettings.maxChannels, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr),
		"Failed to initialize FMOD system.");

	loadBank(audioSettings.masterBank);
	loadBank(audioSettings.masterStringBank);

	for (const FilePath& path : audioSettings.bankPaths)
	{
		loadBank(path);
	}
}

UniquePtr<Bus> Audio::getBus(const std::string& busName)
{
	FMOD::Studio::Bus* bus = nullptr;
	ORC_FMOD_CALL(
		m_system->getBus(busName.c_str(), &bus),
		"Failed to get bus '{}'.", busName);

	return createUniquePtr<Bus>(bus);
}

void Audio::loadBank(const FilePath& filePath)
{
	FMOD::Studio::Bank* bank = nullptr;
	ORC_FMOD_CALL(
		m_system->loadBankFile(filePath.string().c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank),
		"Failed to load audio bank '{}'.", filePath.string());
	ORC_FMOD_CALL(
		bank->loadSampleData(),
		"Failed to load sample data for audio bank '{}'.", filePath.string());

	m_banks.push_back(bank);
}

Audio::~Audio()
{
	ORC_LOG_INFO("Deinitializing audio.");

	for (auto* bank : m_banks)
	{
		ORC_FMOD_CALL(
			bank->unloadSampleData(),
			"Failed to unload data from audio bank.");
		ORC_FMOD_CALL(
			bank->unload(),
			"Failed to unload audio bank.");
	}

	ORC_FMOD_CALL(
		m_system->release(),
		"Failed to deinitialize audio.");
}

void Audio::play(const std::string& eventPath)
{
	FMOD::Studio::EventDescription* eventDescription = nullptr;
	ORC_FMOD_CALL(
		m_system->getEvent(eventPath.c_str(), &eventDescription),
		"Failed to find audio event '{}'.", eventPath);

	FMOD::Studio::EventInstance* eventInstance = nullptr;
	ORC_FMOD_CALL(
		eventDescription->createInstance(&eventInstance),
		"Failed to create event instance for '{}'.", eventPath);

	ORC_FMOD_CALL(
		eventInstance->start(),
		"Failed to start event instance for '{}'.", eventPath);
	ORC_FMOD_CALL(
		eventInstance->release(),
		"Failed to release event instance for '{}'.", eventPath);
}

void Audio::update()
{
	ORC_FMOD_CALL(
		m_system->update(),
		"Audio system update failed.");
}

}
