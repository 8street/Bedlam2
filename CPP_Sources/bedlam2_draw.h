#pragma once
#include <cstdint>

#define SIDEBAR_WIDTH 160
#define SIDEBAR_HEIGHT 480

extern "C" uint8_t SIDEBAR_SURFACE_ARR[409600];
extern "C" uint8_t *SCREEN_BUFFER_PTR;
extern uint8_t GAME_SCREEN_PTR[409600];
extern "C" volatile uint32_t SCREEN_SURFACE_WIDTH;
extern volatile uint32_t SCREEN_SURFACE_HEIGHT;

extern "C" void clear_screen();
extern "C" void copy_buffer_to_screen_and_unlock(uint8_t *buffer);
extern "C" void redraw();
extern "C" int unlock_surface_and_wait(int time_to_waiting);
extern "C" void redraw_copy_buff_to_screen_and_unlock(uint8_t *screen_buf);
extern "C" void draw_map_and_game_screen();
extern "C" void copy_screen_to_buffer(uint8_t *buffer_ptr);
extern "C" void get_screen_buffer_ptr();
