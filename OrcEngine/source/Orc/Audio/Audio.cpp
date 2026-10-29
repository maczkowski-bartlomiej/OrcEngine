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
#include "Core/Debug.hpp"
#include "Core/Logger.hpp"

namespace orc {

namespace {

constexpr std::string_view speakerModeToString(FMOD_SPEAKERMODE mode) noexcept
{
	switch (mode)
	{
		case FMOD_SPEAKERMODE_DEFAULT:
			return "Default";
		case FMOD_SPEAKERMODE_RAW:
			return "Raw";
		case FMOD_SPEAKERMODE_MONO:
			return "Mono";
		case FMOD_SPEAKERMODE_STEREO:
			return "Stereo";
		case FMOD_SPEAKERMODE_QUAD:
			return "Quad";
		case FMOD_SPEAKERMODE_SURROUND:
			return "Surround";
		case FMOD_SPEAKERMODE_5POINT1:
			return "5.1 Surround";
		case FMOD_SPEAKERMODE_7POINT1:
			return "7.1 Surround";
		case FMOD_SPEAKERMODE_7POINT1POINT4:
			return "7.1.4 Surround";
		default:
			return "Unknown";
	}
}

}

Audio::Audio(const AudioSettings& audioSettings)
{
	ORC_LOG_INFO("Initializing audio.");

	ORC_FMOD_CALL(
		FMOD::Studio::System::create(&m_system),
		"Failed to create FMOD system.");
	ORC_FMOD_CALL(
		m_system->initialize(audioSettings.maxChannels, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr),
		"Failed to initialize FMOD system.");

	FMOD::System* coreSystem = nullptr;
	ORC_FMOD_CALL(
		m_system->getCoreSystem(&coreSystem),
		"Failed to get FMOD core system.");

	unsigned int version = 0;
	ORC_FMOD_CALL(
		coreSystem->getVersion(&version),
		"Failed to get FMOD version.");

	int driverId = 0;
	ORC_FMOD_CALL(
		coreSystem->getDriver(&driverId),
		"Failed to get active FMOD driver.");

	char driverName[256] = {};
	int systemRate = 0;
	FMOD_SPEAKERMODE speakerMode = FMOD_SPEAKERMODE_DEFAULT;
	ORC_FMOD_CALL(
		coreSystem->getDriverInfo(driverId, driverName, sizeof(driverName), nullptr, &systemRate, &speakerMode, nullptr),
		"Failed to get FMOD driver info.");

	loadBank(audioSettings.masterBank);
	loadBank(audioSettings.masterStringBank);

	for (const FilePath& path : audioSettings.bankPaths)
	{
		loadBank(path);
	}

	ORC_LOG_INFO("Audio:\n\tFMOD Studio Version: {}.{:02x}.{:02x}\n\tOutput Device: {}\n\tSample Rate: {} Hz\n\tSpeaker Mode: {}\n\tMax Channels: {}\n\tLoaded Banks: {}", (version >> 16) & 0xffff, (version >> 8) & 0xff, version & 0xff, driverName, systemRate, speakerModeToString(speakerMode), audioSettings.maxChannels, m_banks.size());
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
