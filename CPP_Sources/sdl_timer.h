#pragma once
#include <SDL.h>
#include <cstdint>

extern "C" volatile int32_t WAITING_TIMER;
extern "C" volatile int32_t GAME_UPDATE_TIMER;
extern "C" void dos_sleep(uint32_t time);

class Timer
{
public:
    Timer(int interval_ms = 10);
    ~Timer();

    int init(int interval_ms);
    int init();

private:
    int m_interval_ms = 0;
    SDL_TimerID m_timer_id = 0;
};

extern Timer GAME_TIMER;