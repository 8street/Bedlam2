#pragma once
#include <cstdint>

// This defines here not need to modify

#define ORIGINAL_GAME_WIDTH 640
#define ORIGINAL_GAME_HEIGHT 480

#define SIDEBAR_WIDTH 160
#define SIDEBAR_HEIGHT 480

#define TILE_WIDTH 64
#define TILE_HEIGHT 32

#define MAP_WIDTH 480
#define MAP_HEIGHT 480

// Index8 buffer 640*480 with sidebar on right side. Should be in original resolution
extern "C" uint8_t *SIDEBAR_BUFFER_PTR;
// Index8 buffer 640*480 with map on left side. Should be in original resolution
extern "C" uint8_t *MAP_BUFFER_PTR;
// Index8 buffer game_x*game_y with visible screen data
extern "C" uint8_t *SCREEN_BUFFER_PTR;
// Index8 buffer (game_x+dx)*(game_y+dy)
// Game render sprites in this buffer in game level then copy this buffer to screen to avoid black holes on the edge
extern "C" uint8_t *GAME_SCREEN_PTR;

extern "C" volatile int32_t GAME_SCREEN_WIDTH;
extern "C" volatile int32_t GAME_SCREEN_SIZE;
extern "C" volatile int32_t LIMIT_GAME_SCREEN_WIDTH;
extern "C" volatile int32_t LIMIT_GAME_SCREEN_HEIGHT;

extern "C" volatile uint32_t SCREEN_SURFACE_WIDTH;
extern "C" volatile uint32_t SCREEN_SURFACE_HEIGHT;

extern "C" int32_t VETRICAL_TILE_LEVEL_OFFSET;
extern "C" int32_t NUM_TILES_X;
extern "C" int32_t NUM_TILES_Y;
extern "C" int32_t NOT_VISIBLE_TILES_X;
extern "C" int32_t NOT_VISIBLE_TILES_Y;
extern "C" int32_t TILES_START_POS_X;
extern "C" int32_t TILES_START_POS_Y;
extern "C" uint32_t *Z_BUFFER_PTR;

extern int32_t DISPLACE_SCREEN_X;
extern int32_t DISPLACE_SCREEN_Y;

// Displace vertical robot origin for correct robot top part angle depending on mouse position
extern "C" int32_t DISPLACE_ROBOT_ORIGIN_Y;

extern "C" int32_t SIDEBAR_START_POS_X;

extern "C" void clear_screen();
extern "C" void copy_buffer_to_screen_and_unlock(uint8_t *buffer);
extern "C" void redraw();
extern "C" int unlock_surface_and_wait(int time_to_waiting);
extern "C" void redraw_copy_buff_to_screen_and_unlock(uint8_t *screen_buf);
extern "C" void draw_map_and_game_screen();
extern "C" void copy_screen_to_buffer(uint8_t *buffer_ptr);
extern "C" void get_screen_buffer_ptr();
extern "C" void correct_screen_pos();