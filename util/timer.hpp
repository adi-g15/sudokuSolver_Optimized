#pragma once

#include <iostream>
#include <chrono>
#include <thread>

namespace util
{
    struct timer;
    struct scopetimer;
} // namespace util

//DEFINITIONS
struct util::timer
{
    std::chrono::high_resolution_clock::time_point begin;

public:
    inline void start()
    {
        this->begin = std::chrono::high_resolution_clock::now();
    }
    inline int64_t stop()
    {
        int64_t dur = (std::chrono::high_resolution_clock::now() - this->begin).count();
        std::clog << "Took " << dur << " nanoseconds\n";
        return dur;
    }
};

struct util::scopetimer
{
    std::chrono::high_resolution_clock::time_point begin;

public:
    scopetimer() : begin(std::chrono::high_resolution_clock::now())
    {
    }

    ~scopetimer()
    {
        int64_t dur = (std::chrono::high_resolution_clock::now() - this->begin).count();
        std::clog << "Took " << dur << " nanoseconds\n";
    }
};
