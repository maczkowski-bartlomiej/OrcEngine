#pragma once

#include <string_view>

namespace orc {

class Audio;

class Bus
{
public:
	static constexpr std::string_view Master = "bus:/";
	static constexpr std::string_view SFX = "bus:/SFX";
	static constexpr std::string_view Music = "bus:/Music";

	Bus() = default;

	void stop();
	void pause();
	void resume();

	float getVolume() const;
	void setVolume(float volume);

	bool isPaused() const;

	bool isValid() const;

private:
	friend class Audio;
	explicit Bus(void* nativeBus);

	void* m_nativeBus = nullptr;
};

} // namespace orc
