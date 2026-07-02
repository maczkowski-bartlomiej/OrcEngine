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

private:
    time_point m_time = clock::now();
};

}
