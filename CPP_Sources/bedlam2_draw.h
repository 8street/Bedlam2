#pragma once
#include <cstdint>

extern "C" void clear_screen();
extern "C" void copy_buffer_to_screen_and_unlock(uint8_t *buffer);
extern "C" void redraw();
extern "C" int unlock_surface_and_wait(int time_to_waiting);
extern "C" void redraw_copy_buff_to_screen_and_unlock(uint8_t *screen_buf);
extern "C" void draw_map_and_game_screen();
extern "C" void copy_screen_to_buffer(uint8_t *buffer_ptr);
extern "C" void get_screen_buffer_ptr();
