#pragma once

#include <SDL.h>
#include <cstdint>

extern SDL_Surface *SCREEN_SURFACE;

extern "C" uint8_t *SCREEN_BUFFER_PTR;
extern uint8_t GAME_SCREEN_PTR[409600];

int init_video();
int sdl_set_palette(uint8_t *pal_ptr, int offset, int num_entries);
extern "C" void clear_screen();
extern "C" void copy_buffer_to_screen_and_unlock(uint8_t *buffer);
extern "C" void redraw();
extern "C" int unlock_surface_and_wait(int time_to_waiting);

extern "C" void redraw_copy_buff_to_screen_and_unlock(uint8_t *screen_buf);
extern "C" void draw_map_and_game_screen();
int clear_game_viewport();
extern "C" void draw_map_and_game_screen();
void dead_screen_scaler();
extern "C" void copy_screen_to_buffer(uint8_t *buffer_ptr);
SDL_Texture *MY_CreateTextureFromSurface(SDL_Renderer *renderer, const SDL_Surface *surface);