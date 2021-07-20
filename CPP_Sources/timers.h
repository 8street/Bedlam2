#pragma once
#include <Windows.h>
#include <stdint.h>

extern uint32_t TIMER_RESOLUTION;
extern uint32_t TIMER_EVENT;

extern "C" volatile int32_t WAITING_TIMER;
extern "C" volatile int32_t GAME_UPDATE_TIMER;

int init_timer();
void timer_callback(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
void timer_update();
void increment_timers();
void timer_deinit();
extern "C" void dos_sleep(uint32_t time);