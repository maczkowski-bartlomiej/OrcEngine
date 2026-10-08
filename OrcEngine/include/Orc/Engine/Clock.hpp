#pragma once

#include <chrono>
#include <cstdint>

namespace orc {

class Clock
{
public:
    using clock = std::chrono::steady_clock;
    using time_point = clock::time_point;

    float elapsed() const noexcept
    {
        const auto currentTime = clock::now();
        return std::chrono::duration<float>(currentTime - m_time).count();
    }

    uint64_t elapsedMs() const noexcept
    {
        const auto currentTime = clock::now();
        return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_time).count());
    }

    void reset() noexcept
    {
        m_time = clock::now();
    }

    float restart() noexcept
    {
        const auto currentTime = clock::now();
        const float elapsedSeconds = std::chrono::duration<float>(currentTime - m_time).count();
        m_time = currentTime;
        return elapsedSeconds;
    }

private:
    time_point m_time = clock::now();
};

}
