#pragma once
#include <cstdint>
////////////////////////////////////////////////////
// Here function prototypes from bedlam2.asm file //
// Here extern variables from bedlam2.asm file    //
////////////////////////////////////////////////////

extern "C" {
int __cdecl main_(int, const char **, const char **);
void level_clock();
}

extern "C" volatile int32_t screen_x_pos;
extern "C" volatile int32_t screen_y_pos;
extern "C" volatile int32_t game_is_playing;
extern "C" volatile int32_t map_active;
extern "C" volatile int32_t dead_screen_scale;