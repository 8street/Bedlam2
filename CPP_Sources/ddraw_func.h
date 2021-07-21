#pragma once

#include <Windows.h>
#include <ddraw.h>
#include <stdint.h>

extern int32_t GAME_WIDTH;
extern int32_t GAME_HEIGHT;

extern volatile uint8_t FULLSCREEN;
extern volatile uint8_t SURFACE_IS_LOCKED;

extern volatile uint16_t USE_VIDEOMEMORY;

extern LPDIRECTDRAWCLIPPER DDRAW_SURFACE_CLIPPER;

extern PALETTEENTRY PALETTEENTRY_BUFFER[258];

extern "C" volatile uint32_t SCREEN_SURFACE_WIDTH;
extern volatile uint32_t SCREEN_SURFACE_HEIGHT;



extern volatile uint8_t IS_BLITTING;

HRESULT unlock_surface();
uint8_t *lock_and_get_surface_ptr();
int blit_cursor_bg_to_screen();
void blit_cursor_bg();
int blit_second_surface_to_screen();
int blit_cursor(int x, int y);
void activate_app(int32_t activate);
extern "C" void get_screen_buffer_ptr();
HRESULT unlock_surface_and_screen_ptr();
HRESULT ddraw_setpalettes(uint8_t *pal_ptr, int16_t offset, int16_t num_entries);

void unlock_cursor_surface();
uint8_t *lock_and_get_cursor_surface();
HRESULT Set_Cursor_ColorKey(int16_t color_range);



void ddraw_deinit();
