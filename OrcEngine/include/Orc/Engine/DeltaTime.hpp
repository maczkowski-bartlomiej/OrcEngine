#include <chrono>

namespace orc {

class DeltaTime
{
public:
    DeltaTime() 
    {
        m_time = std::chrono::steady_clock::now();
    }

    double elapsed() const
    {
        auto currentTime = std::chrono::steady_clock::now();
        return std::chrono::duration<double>(currentTime - m_time).count();
    }

    void reset() 
    {
        m_time = std::chrono::steady_clock::now();
    }

private:
    std::chrono::time_point<std::chrono::steady_clock> m_time;
};

}
