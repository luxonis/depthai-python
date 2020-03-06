#pragma once

#include <chrono>
#include <ratio>


class Timer
{
public:
    Timer()
    {
        _start = std::chrono::high_resolution_clock::now();
    }

    unsigned long ellapsed_ms()
    {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now - _start).count();
    }

    unsigned long ellapsed_us()
    {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(now - _start).count();
    }

    unsigned long ellapsed_ns()
    {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::nanoseconds>(now - _start).count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _start;
};
