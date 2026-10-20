#include "OrcPch.hpp"

#include "Audio/Audio.hpp"

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include <fmod_common.h>
#include <fmod_studio.hpp>
#include <fmod_studio_common.h>

#include "Audio/Bus.hpp"
#include "Audio/SoundHandle.hpp"
#include "AudioUtility.hpp"
#include "Core/Core.hpp"
#include "Engine/Debug.hpp"
#include "Engine/Logger.hpp"

namespace orc {

struct Audio::Impl
{
	FMOD::Studio::System* system = nullptr;
	std::vector<FMOD::Studio::Bank*> banks;

	struct SoundSlot
	{
		uint32_t generation = 1;
		FMOD::Studio::EventInstance* instance = nullptr;
	};

	std::vector<SoundSlot> soundPool;
	std::vector<uint32_t> freeSlots;

	std::unordered_map<std::string, FMOD::Studio::Bus*> buses;
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> eventDescriptions;

	FMOD::Studio::EventDescription* getEventDescription(std::string_view eventPath)
	{
		std::string path(eventPath);
		auto it = eventDescriptions.find(path);
		if (it != eventDescriptions.end())
		{
			return it->second;
		}

		FMOD::Studio::EventDescription* eventDescription = nullptr;
		if (!fmodCall(system->getEvent(path.c_str(), &eventDescription)))
		{
			ORC_LOG_ERROR("Failed to find audio event '{}'.", eventPath);
		}
		ORC_ASSERT(eventDescription, "FMOD returned empty event '{}'.", eventPath);

		eventDescriptions.emplace(std::move(path), eventDescription);
		return eventDescription;
	}

	FMOD::Studio::EventInstance* createStartedInstance(std::string_view eventPath)
	{
		FMOD::Studio::EventDescription* eventDescription = getEventDescription(eventPath);
		ORC_ASSERT(eventDescription, "Audio event '{}' does not exist.", eventPath);

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

		return eventInstance;
	}
};

Audio::Audio(const AudioSettings& audioSettings)
	: m_impl(createUniquePtr<Impl>())
{
	ORC_LOG_INFO("Initializing audio.");

	if (!fmodCall(FMOD::Studio::System::create(&m_impl->system)))
	{
		ORC_LOG_FATAL("Failed to create FMOD system.");
	}

	if (!fmodCall(m_impl->system->initialize(audioSettings.maxChannels, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr)))
	{
		ORC_LOG_FATAL("Failed to initialize FMOD system.");
	}

	for (const auto& audioBank : audioSettings.banks)
	{
		loadBank(audioBank);
	}
}

Audio::~Audio()
{
	ORC_LOG_INFO("Deinitializing audio.");

	if (!m_impl || !m_impl->system)
	{
		return;
	}

	for (auto& slot : m_impl->soundPool)
	{
		if (slot.instance)
		{
			(void)fmodCall(slot.instance->stop(FMOD_STUDIO_STOP_IMMEDIATE));
			(void)fmodCall(slot.instance->release());
			slot.instance = nullptr;
		}
	}
	m_impl->soundPool.clear();
	m_impl->freeSlots.clear();

	for (auto* bank : m_impl->banks)
	{
		if (!bank)
		{
			continue;
		}

		(void)fmodCall(bank->unloadSampleData());
		(void)fmodCall(bank->unload());
	}
	m_impl->banks.clear();

	if (!fmodCall(m_impl->system->release()))
	{
		ORC_LOG_ERROR("Failed to deinitialize audio.");
	}
}

bool Audio::loadBank(const FilePath& filePath)
{
	ORC_ASSERT(std::filesystem::exists(filePath), "Attempted to load an audio bank from a non-existent file '{}'.", filePath.string());
	ORC_ASSERT(m_impl && m_impl->system, "Attempted to load an audio bank with an uninitialized audio system.");

	FMOD::Studio::Bank* bank = nullptr;
	bool loadBankSuccess = fmodCall(m_impl->system->loadBankFile(filePath.string().c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank)) && bank;
	ORC_ASSERT(loadBankSuccess, "Failed to load audio bank '{}'.", filePath.string());

	bool loadSampleSuccess = fmodCall(bank->loadSampleData());
	ORC_ASSERT(loadSampleSuccess, "Failed to load sample data for audio bank '{}'.", filePath.string());

	m_impl->banks.push_back(bank);
	return true;
}

void Audio::playOneShot(std::string_view eventPath)
{
	ORC_ASSERT(m_impl && m_impl->system, "Uninitialized audio system.");

	FMOD::Studio::EventInstance* eventInstance = m_impl->createStartedInstance(eventPath);

	if (!fmodCall(eventInstance->release()))
	{
		ORC_LOG_ERROR("Failed to release event instance for '{}'.", eventPath);
	}
}

SoundHandle Audio::play(std::string_view eventPath)
{
	ORC_ASSERT(m_impl && m_impl->system, "Uninitialized audio system.");

	FMOD::Studio::EventInstance* eventInstance = m_impl->createStartedInstance(eventPath);

	uint32_t index = 0;
	if (!m_impl->freeSlots.empty())
	{
		index = m_impl->freeSlots.back();
		m_impl->freeSlots.pop_back();
		m_impl->soundPool[index].instance = eventInstance;
	}
	else
	{
		index = static_cast<uint32_t>(m_impl->soundPool.size());
		m_impl->soundPool.push_back(Impl::SoundSlot{1, eventInstance});
	}

	return SoundHandle(*this, index, m_impl->soundPool[index].generation);
}

void Audio::update()
{
	ORC_ASSERT(m_impl && m_impl->system, "Uninitialized audio system.");

	if (!fmodCall(m_impl->system->update()))
	{
		ORC_LOG_ERROR("Audio system update failed.");
	}

	for (uint32_t i = 0; i < static_cast<uint32_t>(m_impl->soundPool.size()); ++i)
	{
		auto& slot = m_impl->soundPool[i];
		if (!slot.instance)
		{
			continue;
		}

		FMOD_STUDIO_PLAYBACK_STATE state = FMOD_STUDIO_PLAYBACK_STOPPED;
		if (fmodCall(slot.instance->getPlaybackState(&state)))
		{
			if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
			{
				(void)fmodCall(slot.instance->release());
				slot.instance = nullptr;
				slot.generation++;
				if (slot.generation == 0)
				{
					slot.generation = 1;
				}
				m_impl->freeSlots.push_back(i);
			}
		}
	}
}

Bus Audio::getBus(std::string_view busPath)
{
	ORC_ASSERT(m_impl && m_impl->system, "Uninitialized audio system.");

	std::string path(busPath);
	auto it = m_impl->buses.find(path);
	if (it != m_impl->buses.end())
	{
		return Bus(it->second);
	}

	FMOD::Studio::Bus* fmodBus = nullptr;
	if (!fmodCall(m_impl->system->getBus(path.c_str(), &fmodBus)))
	{
		ORC_LOG_ERROR("Failed to find audio bus '{}'.", busPath);
	}
	ORC_ASSERT(fmodBus, "Audio bus '{}' does not exist in audio system.", busPath);

	m_impl->buses.emplace(std::move(path), fmodBus);
	return Bus(fmodBus);
}

bool Audio::isSoundValid(uint32_t index, uint32_t generation) const
{
	if (!m_impl || index >= m_impl->soundPool.size())
	{
		return false;
	}

	const auto& slot = m_impl->soundPool[index];
	return slot.instance != nullptr && slot.generation == generation;
}

void Audio::stopSound(uint32_t index, uint32_t generation, bool immediate)
{
	if (!isSoundValid(index, generation))
	{
		return;
	}

	auto* instance = m_impl->soundPool[index].instance;
	FMOD_STUDIO_STOP_MODE mode = immediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
	if (!fmodCall(instance->stop(mode)))
	{
		ORC_LOG_ERROR("Failed to stop sound instance.");
	}
}

void Audio::pauseSound(uint32_t index, uint32_t generation)
{
	if (!isSoundValid(index, generation))
	{
		return;
	}

	auto* instance = m_impl->soundPool[index].instance;
	if (!fmodCall(instance->setPaused(true)))
	{
		ORC_LOG_ERROR("Failed to pause sound instance.");
	}
}

void Audio::resumeSound(uint32_t index, uint32_t generation)
{
	if (!isSoundValid(index, generation))
	{
		return;
	}

	auto* instance = m_impl->soundPool[index].instance;
	if (!fmodCall(instance->setPaused(false)))
	{
		ORC_LOG_ERROR("Failed to resume sound instance.");
	}
}

void Audio::setSoundVolume(uint32_t index, uint32_t generation, float volume)
{
	if (!isSoundValid(index, generation))
	{
		return;
	}

	auto* instance = m_impl->soundPool[index].instance;
	if (!fmodCall(instance->setVolume(volume)))
	{
		ORC_LOG_ERROR("Failed to set volume on sound instance.");
	}
}

void Audio::setSoundPitch(uint32_t index, uint32_t generation, float pitch)
{
	if (!isSoundValid(index, generation))
	{
		return;
	}

	auto* instance = m_impl->soundPool[index].instance;
	if (!fmodCall(instance->setPitch(pitch)))
	{
		ORC_LOG_ERROR("Failed to set pitch on sound instance.");
	}
}

void Audio::setSoundParameter(uint32_t index, uint32_t generation, std::string_view name, float value)
{
	if (!isSoundValid(index, generation))
	{
		return;
	}

	auto* instance = m_impl->soundPool[index].instance;
	std::string paramName(name);
	if (!fmodCall(instance->setParameterByName(paramName.c_str(), value)))
	{
		ORC_LOG_ERROR("Failed to set parameter '{}' on sound instance.", name);
	}
}

} // namespace orc
