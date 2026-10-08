#include "OrcPch.hpp"
#include "Audio/Audio.hpp"
#include "Audio/Bus.hpp"
#include "Engine/Core.hpp"
#include "Engine/Debug.hpp"
#include "Engine/Logger.hpp"
#include "Engine/Utility.hpp"

#include <fmod_common.h>
#include <fmod_studio.hpp>
#include <fmod_studio_common.h>

#include <filesystem>
#include <string>
#include <vector>

namespace orc {

	Audio::Audio(const AudioSettings& audioSettings)
	{
		ORC_LOG_INFO("Initializing audio.");

		if(!fmodCall(FMOD::Studio::System::create(&m_system)))
		{
			ORC_LOG_FATAL("Failed to create FMOD system.");
		}

		if (!fmodCall(m_system->initialize(audioSettings.maxChannels, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0)))
		{
			ORC_LOG_FATAL("Failed to initialize FMOD system.");
		}

		for (const auto& audioBank : audioSettings.banks)
		{
			loadBank(audioBank);
		}

		m_masterBus.load(m_system, audioSettings.masterBusName);
		m_musicBus.load(m_system, audioSettings.musicBusName);
		m_sfxBus.load(m_system, audioSettings.sfxBusName);
	}

	bool Audio::loadBank(const FilePath& filePath)
	{
		ORC_ASSERT(std::filesystem::exists(filePath), "Attempted to load an audio bank from a non-existent file '{}'.", filePath.string());
		ORC_ASSERT(m_system, "Attempted to load an audio bank with an uninitialized audio system.");

		FMOD::Studio::Bank* bank = nullptr;
		if(!fmodCall(m_system->loadBankFile(filePath.string().c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank)))
		{
			ORC_LOG_ERROR("Failed to load audio bank '{}'.", filePath.string());
			return false;
		}

		if (!fmodCall(bank->loadSampleData()))
		{
			ORC_LOG_ERROR("Failed to load sample data for audio bank '{}'.", filePath.string());
			return false;
		}

		m_banks.push_back(bank);
		return true;
	}

	Audio::~Audio()
	{
		ORC_LOG_INFO("Deinitializing audio.");

		ORC_ASSERT(m_system, "Uninitialized audio system.");

		for (auto* bank : m_banks)
		{
			if (!bank)
				continue;

			(void)fmodCall(bank->unloadSampleData());
			(void)fmodCall(bank->unload());
		}
		m_banks.clear();

		if (!fmodCall(m_system->release()))
		{
			ORC_LOG_ERROR("Failed to deinitialize audio.");
		}
	}

	void Audio::play(const std::string& eventPath)
	{
		ORC_ASSERT(m_system, "Uninitialized audio system.");

		FMOD::Studio::EventDescription* eventDescription = nullptr;
		if (!fmodCall(m_system->getEvent(eventPath.c_str(), &eventDescription)))
		{
			ORC_LOG_ERROR("Failed to find audio event '{}'.", eventPath);
		}
		ORC_ASSERT(eventDescription, "FMOD returned empty event '{}'.", eventPath);

		FMOD::Studio::EventInstance* eventInstance = nullptr;
        if (!fmodCall(eventDescription->createInstance(&eventInstance)))
		{
			ORC_LOG_ERROR("Failed to create event instance for '{}'.", eventPath);
		}
		ORC_ASSERT(eventInstance, "FMOD returned empty event instance for '{}'.", eventPath);

		if (!fmodCall(eventInstance->start()))
		{
			ORC_LOG_ERROR("Failed to start event instance for '{}'.", eventPath);
		}

		if (!fmodCall(eventInstance->release()))
		{
			ORC_LOG_ERROR("Failed to release event instance for '{}'.", eventPath);
		}
	}

	void Audio::update()
	{
		ORC_ASSERT(m_system, "Uninitialized audio system.");

		if (!fmodCall(m_system->update()))
		{
			ORC_LOG_ERROR("Audio system update failed.");
		}
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
