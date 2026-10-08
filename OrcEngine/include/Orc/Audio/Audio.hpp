#pragma once

#include "Audio/Bus.hpp"
#include "Engine/Core.hpp"

#include <string>
#include <string_view>
#include <vector>

#include <fmod_studio.hpp>

namespace orc {

class Audio
{
public:
	struct AudioSettings
	{
		int maxChannels = 512;
		std::string masterBusName = "bus:/";
		std::string sfxBusName = "bus:/SFX";	
		std::string musicBusName = "bus:/Music";
		std::vector<FilePath> banks = {
			"assets/audio/Master.bank",
			"assets/audio/Master.strings.bank",
			"assets/audio/Music.bank",
			"assets/audio/SFX.bank"
		};
	};	

	Audio() = delete;
	Audio(const AudioSettings& audioSettings);
	~Audio();

	Audio(const Audio&) = delete;
	Audio& operator=(const Audio&) = delete;
	Audio(Audio&&) = delete;
	Audio& operator=(Audio&&) = delete;

	bool loadBank(const FilePath& filePath);

	void play(const std::string& eventPath);
	void update();

	Bus& getSfxBus();
	Bus& getMusicBus();
	Bus& getMasterBus();

private:
	Bus m_sfxBus;
	Bus m_musicBus;
	Bus m_masterBus;

	FMOD::Studio::System* m_system = nullptr;
    std::vector<FMOD::Studio::Bank*> m_banks;
};

}
