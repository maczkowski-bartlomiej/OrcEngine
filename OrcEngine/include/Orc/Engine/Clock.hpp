#pragma once

#include <chrono>

namespace orc {

class Clock
{
public:
    Clock()
    {
        m_time = std::chrono::steady_clock::now();
    }

    float elapsed() const
    {
        auto currentTime = std::chrono::steady_clock::now();
        return std::chrono::duration<float>(currentTime - m_time).count();
    }

    uint64_t elapsedMs() const {
        auto currentTime = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_time).count();
    }

    void reset() 
    {
        m_time = std::chrono::steady_clock::now();
    }

private:
    std::chrono::time_point<std::chrono::steady_clock> m_time;
};

}
