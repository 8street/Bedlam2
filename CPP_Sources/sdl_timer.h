#pragma once

extern "C" volatile int32_t WAITING_TIMER;
extern "C" volatile int32_t GAME_UPDATE_TIMER;

int init_timer();
Uint32 sdl_timer_callback(Uint32 interval, void *param);
extern "C" void dos_sleep(uint32_t time);