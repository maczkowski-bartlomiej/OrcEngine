#pragma once

#include <chrono>
#include <cstdint>

namespace orc {

class Clock
{
public:
	using ClockType = std::chrono::steady_clock;
	using TimePoint = ClockType::time_point;

	float elapsed() const noexcept
	{
		const auto currentTime = ClockType::now();
		return std::chrono::duration<float>(currentTime - m_time).count();
	}

	uint64_t elapsedMs() const noexcept
	{
		const auto currentTime = ClockType::now();
		return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_time).count());
	}

	void reset() noexcept
	{
		m_time = ClockType::now();
	}

	float restart() noexcept
	{
		const auto currentTime = ClockType::now();
		const float elapsedSeconds = std::chrono::duration<float>(currentTime - m_time).count();
		m_time = currentTime;
		return elapsedSeconds;
	}

private:
	TimePoint m_time = ClockType::now();
};

}
