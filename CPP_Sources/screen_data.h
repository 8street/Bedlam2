#pragma once
#include <SDL.h>
#include <cstdint>
#include <vector>

#include "sdl_texture.h"

/// <summary>
/// A class that binds a old output DOS format and a state of art texture format.
/// m_screen_surface - screen surface in index8 DOS format. This is not accelerated, and slow for rendering.
/// m_screen_texture - screen texture in videomemory prepared for rendering in RGBA32 format. Very fast output.
/// </summary>
class Screen_data
{
public:
    Screen_data(uint8_t *screen_buffer, int surface_width, int surface_height);
    Screen_data();
    ~Screen_data();

    int init(uint8_t *screen_buffer, int surface_width, int surface_height);
    int set_palette(uint8_t *pal_ptr, int offset, int num_entries);
    uint8_t *get_RGB_palette_ptr();
    int fill_screen_surface(uint8_t *buffer_ptr);
    int update_texture();
    uint8_t *lock_and_get_surface_ptr();
    int unlock_surface() const;
    int clear();
    SDL_Texture *get_texture();
    int copy_surface_to_buffer(uint8_t *buffer_ptr);
    int get_surface_width() const;
    int get_surface_height() const;

private:
    int destroy();
    SDL_Surface *m_screen_surface = nullptr;
    bool m_must_lock_surface = false;
    Texture m_screen_texture;
    SDL_Rect m_texture_coord_in_surface = { 0 };
};
