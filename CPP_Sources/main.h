#pragma once
#include <Windows.h>
#include <stdint.h>

extern DWORD GAME_THREAD_ID;
extern HANDLE HANDLE_GAME_THREAD;

int16_t game_thread_create();
void game_thread_start_func();
void game_thread_exit();
extern "C" void free_win();