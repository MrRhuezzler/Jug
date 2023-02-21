#include "jug/timer.h"

namespace Jug
{
    Timer::Timer()
    {
        reset();
    }

    Timer::~Timer()
    {
    }

    float Timer::getTimeElapsed()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count() * 0.001f * 0.001f * 0.001f;
    }

    float Timer::getTimeElapsedMillis()
    {
        return getTimeElapsed() * 1000.0f;
    }

    void Timer::reset()
    {
        start = std::chrono::high_resolution_clock::now();
    }
} // namespace Jug