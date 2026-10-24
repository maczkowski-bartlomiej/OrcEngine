#include "OrcPch.hpp"

#include "Audio/Audio.hpp"

#include <filesystem>
#include <string>
#include <vector>

#include <fmod_common.h>
#include <fmod_studio.hpp>
#include <fmod_studio_common.h>

#include "Audio/Bus.hpp"
#include "Core/Core.hpp"
#include "Engine/Debug.hpp"
#include "Engine/Logger.hpp"
#include "Engine/Utility.hpp"

namespace orc {

Audio::Audio(const AudioSettings& audioSettings)
{
	ORC_LOG_INFO("Initializing audio...");

	fmodCall(FMOD::Studio::System::create(&m_system), "Failed to create FMOD system");
	fmodCall(m_system->initialize(audioSettings.maxChannels, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr), "Failed to initialize FMOD system");

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
	fmodCall(m_system->getBus(busName.c_str(), &bus), fmt::format("Failed to get bus '{}'.", busName));
	ORC_ASSERT(bus, "FMOD returned empty bus '{}'.", busName);

	return createUniquePtr<Bus>(bus);
}

void Audio::loadBank(const FilePath& filePath)
{
	ORC_ASSERT(std::filesystem::exists(filePath), "Attempted to load an audio bank from a non-existent file '{}'.", filePath.string());

	FMOD::Studio::Bank* bank = nullptr;
	fmodCall(m_system->loadBankFile(filePath.string().c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank), fmt::format("Failed to load audio bank '{}'.", filePath.string()));
	fmodCall(bank->loadSampleData(), fmt::format("Failed to load sample data for audio bank '{}'.", filePath.string()));

	m_banks.push_back(bank);
}

Audio::~Audio()
{
	ORC_LOG_INFO("Deinitializing audio.");

	for (auto* bank : m_banks)
	{
		if (!bank)
			continue;

		fmodCall(bank->unloadSampleData(), "Failed to unload data from audio bank.");
		fmodCall(bank->unload(), "Failed to unload audio bank.");
	}

	fmodCall(m_system->release(), "Failed to deinitialize audio.");
}

void Audio::play(const std::string& eventPath)
{
	FMOD::Studio::EventDescription* eventDescription = nullptr;
	fmodCall(m_system->getEvent(eventPath.c_str(), &eventDescription), fmt::format("Failed to find audio event '{}'.", eventPath));
	ORC_ASSERT(eventDescription, "FMOD returned empty event '{}'.", eventPath);

	FMOD::Studio::EventInstance* eventInstance = nullptr;
	fmodCall(eventDescription->createInstance(&eventInstance), fmt::format("Failed to create event instance for '{}'.", eventPath));
	ORC_ASSERT(eventInstance, "FMOD returned empty event instance for '{}'.", eventPath);

	fmodCall(eventInstance->start(), fmt::format("Failed to start event instance for '{}'.", eventPath));
	fmodCall(eventInstance->release(), fmt::format("Failed to release event instance for '{}'.", eventPath));
}

void Audio::update()
{
	fmodCall(m_system->update(), "Audio system update failed.");
}

}
