#pragma once
#include <stdint.h>
#include <random>

class Rnd
{
public:
    Rnd();
    Rnd(uint32_t seed);
    uint32_t get();
    uint32_t get(uint32_t max);
    int32_t get(int32_t min, int32_t max);

private:
    std::mt19937 m_generator;    
};
