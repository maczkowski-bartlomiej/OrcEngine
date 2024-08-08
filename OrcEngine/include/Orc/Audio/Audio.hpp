#pragma once

#include "Engine/Core.hpp"

#include "Audio/Bus.hpp"

#include <queue>
#include <vector>

namespace FMOD::Studio { class System; class EventInstance; }

namespace orc {

class Audio
{
public:
	struct AudioSettings
	{
		int maxChannels = 512;
		std::string sfxBusName = "bus:/SFX";	
		std::string musicBusName = "bus:/Music";
	};	

	Audio(const AudioSettings& audioSettings, const std::vector<std::string>& audioBanks);
	~Audio();

	bool loadBank(const FilePath& filePath);

	void play(const std::string& eventPath);

	void update();

	Bus& getSfxBus();
	Bus& getMusicBus();
	Bus& getMasterBus();

private:
	UniquePtr<Bus> m_sfxBus;
	UniquePtr<Bus> m_musicBus;
	UniquePtr<Bus> m_masterBus;
	FMOD::Studio::System* m_system = nullptr;

};

}
