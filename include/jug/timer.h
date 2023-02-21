#pragma once

#include <chrono>

namespace Jug
{
    class Timer
    {
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start;

    public:
        Timer();
        ~Timer();

        float getTimeElapsed();
        float getTimeElapsedMillis();

    private:
        void reset();
    };
} // namespace Jug
