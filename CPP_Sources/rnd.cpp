#include "rnd.h"

Rnd::Rnd()
{
    std::random_device rd;
    m_generator.seed(rd());
}
Rnd::Rnd(uint32_t seed)
{
    m_generator.seed(seed);
}
uint32_t Rnd::get()
{
    return m_generator();
}
uint32_t Rnd::get(uint32_t max) 
{
    std::uniform_int_distribution<> null_to_max(0, max);
    return null_to_max(m_generator);
}
int32_t Rnd::get(int32_t min, int32_t max)
{
    if (min > max) {
        std::swap(min, max);
    }
    std::uniform_int_distribution<> min_to_max(min, max);
    return min_to_max(m_generator);
}
