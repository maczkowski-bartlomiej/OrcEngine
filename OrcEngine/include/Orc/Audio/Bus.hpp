#pragma once

namespace FMOD::Studio {
class Bus;
}

namespace orc {

class Bus
{
public:
	explicit Bus(FMOD::Studio::Bus* bus);
	~Bus() = default;

	Bus(const Bus&) = delete;
	Bus& operator=(const Bus&) = delete;
	Bus(Bus&&) noexcept = default;
	Bus& operator=(Bus&&) noexcept = default;

	void stop();
	void pause();
	void resume();

	float getVolume() const;
	void setVolume(float volume);

	bool isPaused() const;

private:
	FMOD::Studio::Bus* m_bus = nullptr;
};

}
