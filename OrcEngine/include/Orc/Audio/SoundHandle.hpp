#pragma once

#include <cstdint>
#include <string_view>

namespace orc {

class Audio;

class SoundHandle
{
public:
	SoundHandle() = default;

	bool isValid() const;
	void stop(bool immediate = false);
	void pause();
	void resume();
	void setVolume(float volume);
	void setPitch(float pitch);
	void setParameter(std::string_view name, float value);

private:
	friend class Audio;
	explicit SoundHandle(Audio& audio, uint32_t index, uint32_t generation);

	Audio* m_audio = nullptr;
	uint32_t m_index = 0;
	uint32_t m_generation = 0;
};

} // namespace orc
