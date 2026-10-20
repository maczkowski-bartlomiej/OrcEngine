#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

#include "Audio/Bus.hpp"
#include "Audio/SoundHandle.hpp"
#include "Core/Core.hpp"

namespace orc {

class Audio
{
public:
	struct AudioSettings
	{
		int maxChannels = 512;
		std::vector<FilePath> banks = {
			"assets/audio/Master.bank",
			"assets/audio/Master.strings.bank",
			"assets/audio/Music.bank",
			"assets/audio/SFX.bank"};
	};

	Audio() = delete;
	explicit Audio(const AudioSettings& audioSettings);
	~Audio();

	Audio(const Audio&) = delete;
	Audio& operator=(const Audio&) = delete;
	Audio(Audio&&) = delete;
	Audio& operator=(Audio&&) = delete;

	bool loadBank(const FilePath& filePath);

	void playOneShot(std::string_view eventPath);
	SoundHandle play(std::string_view eventPath);
	void update();

	Bus getBus(std::string_view busPath);

private:
	friend class SoundHandle;

	bool isSoundValid(uint32_t index, uint32_t generation) const;
	void stopSound(uint32_t index, uint32_t generation, bool immediate = false);
	void pauseSound(uint32_t index, uint32_t generation);
	void resumeSound(uint32_t index, uint32_t generation);
	void setSoundVolume(uint32_t index, uint32_t generation, float volume);
	void setSoundPitch(uint32_t index, uint32_t generation, float pitch);
	void setSoundParameter(uint32_t index, uint32_t generation, std::string_view name, float value);

	struct Impl;
	UniquePtr<Impl> m_impl;
};

} // namespace orc
