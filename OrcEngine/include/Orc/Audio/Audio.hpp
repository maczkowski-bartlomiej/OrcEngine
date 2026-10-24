#pragma once

#include <string>
#include <string_view>
#include <vector>

#include <fmod_studio.hpp>

#include "Audio/Bus.hpp"
#include "Core/Core.hpp"

namespace orc {

class Audio
{
public:
	struct AudioSettings
	{
		int maxChannels = 512;

		FilePath masterBank = "assets/audio/Master.bank";
		FilePath masterStringBank = "assets/audio/Master.strings.bank";

		std::vector<FilePath> bankPaths;
	};

	Audio() = delete;
	explicit Audio(const AudioSettings& audioSettings);
	~Audio();

	Audio(const Audio&) = delete;
	Audio& operator=(const Audio&) = delete;
	Audio(Audio&&) = delete;
	Audio& operator=(Audio&&) = delete;

	UniquePtr<Bus> getBus(const std::string& busName);

	void play(const std::string& eventPath);
	void update();

private:
	void loadBank(const FilePath& filePath);

	FMOD::Studio::System* m_system = nullptr;
	std::vector<FMOD::Studio::Bank*> m_banks;
};

}
