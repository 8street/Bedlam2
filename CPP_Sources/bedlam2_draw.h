#pragma once
#include <cstdint>

#define SIDEBAR_WIDTH 160
#define SIDEBAR_HEIGHT 480

// Should be in original 640*480 res
extern "C" uint8_t *SIDEBAR_BUFFER_PTR;
// Should be in original 640*480 res
extern "C" uint8_t *MAP_BUFFER_PTR;
// Should be in user window resolution when game is playing
extern "C" uint8_t *SCREEN_BUFFER_PTR;
// Should be in user window resolution plus one tile porch in from each side to avoid black holes
extern "C" uint8_t *GAME_SCREEN_PTR;

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
