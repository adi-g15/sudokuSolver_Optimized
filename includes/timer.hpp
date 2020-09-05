#pragma once

#include<iostream>
#include<chrono>

struct timer{
    std::chrono::high_resolution_clock::time_point begin;
    public:
        inline void start(){
            auto begin = std::chrono::high_resolution_clock::now();
        }
        inline int64_t stop(){
            int64_t dur = (std::chrono::high_resolution_clock::now() - this->begin).count();
            std::cout<<"Took "<<dur<<" nanoseconds\n";
            return dur;
        }
};
