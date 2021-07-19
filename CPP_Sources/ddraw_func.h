#pragma once

#include <ddraw.h>
#include <stdint.h>
#include <Windows.h>

extern int32_t GAME_WIDTH;
extern int32_t GAME_HEIGHT;

extern volatile uint8_t FULLSCREEN;
extern volatile uint8_t SURFACE_IS_LOCKED;

extern volatile uint16_t USE_VIDEOMEMORY;


extern LPDIRECTDRAWCLIPPER DDRAW_SURFACE_CLIPPER;

extern PALETTEENTRY PALETTEENTRY_BUFFER[258];

extern "C" volatile uint32_t SCREEN_SURFACE_WIDTH;
extern volatile uint32_t SCREEN_SURFACE_HEIGHT;

extern "C" uint8_t* SCREEN_BUFFER_PTR;
extern uint8_t GAME_SCREEN_PTR[409600];


extern volatile uint8_t IS_BLITTING;

int ddraw_init(HWND hWnd);
int create_surface_palette(int32_t width, int32_t height, int32_t depth);
HRESULT create_mouse_buffer();
void clear_and_blit_screen();
HRESULT unlock_surface();
uint8_t* lock_and_get_surface_ptr();
int blit_cursor_bg_to_screen();
void blit_cursor_bg();
int blit_second_surface_to_screen();
int blit_cursor(int x, int y);
void activate_app(int32_t activate);
extern "C" void get_screen_buffer_ptr();
HRESULT unlock_surface_and_screen_ptr();
HRESULT ddraw_setpalettes(uint8_t* pal_ptr, int16_t offset, int16_t num_entries);
extern "C" void redraw();
extern "C" void copy_buffer_to_screen_and_unlock(uint8_t* buffer);
void unlock_cursor_surface();
uint8_t* lock_and_get_cursor_surface();
HRESULT Set_Cursor_ColorKey(int16_t color_range);
extern "C" void clear_screen();
extern "C" int unlock_surface_and_wait(int time_to_waiting);
extern "C" void redraw_copy_buff_to_screen_and_unlock(uint8_t* screen_buf);
extern "C" void draw_map_and_game_screen();
int clear_game_viewport();
extern "C" void draw_map_and_game_screen();
void ddraw_deinit();
void dead_screen_scaler();
extern "C" void copy_screen_to_buffer(uint8_t* buffer_ptr);