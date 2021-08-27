#pragma once
#include <SDL.h>
#include <cstdint>
#include <vector>

#include "sdl_texture.h"
#include "bedlam2_draw.h"

/// <summary>
/// A class that binds a old output DOS format and a state of art texture format.
/// m_surface - screen surface in index8 DOS format. This is not accelerated, and slow for rendering.
/// m_texture - screen texture in videomemory prepared for rendering in RGBA32 format. Very fast output.
/// </summary>
class Screen_data
{
public:
    Screen_data(uint8_t *screen_buffer, int surface_width, int surface_height, int offset_x = 0, int offset_y = 0);
    Screen_data();
    ~Screen_data();
    
    int destroy();
    int init(
        uint8_t *screen_buffer, int surface_width, int surface_height, 
        int img_width = ORIGINAL_GAME_WIDTH, int img_height = ORIGINAL_GAME_HEIGHT,
        int offset_x = 0, int offset_y = 0);
    int set_palette(const uint8_t *pal_ptr, int offset, int num_entries);
    int set_palette(const File &palette_file);
    uint8_t *get_RGB_palette_ptr();
    int fill_screen_surface(uint8_t *buffer_ptr);
    int fill_screen_surface(uint8_t *buffer_ptr, int screen_x, int screen_y, int buffer_x, int buffer_y, int buffer_count_w, int buffer_count_h, int buffer_w);
    int update_texture();
    uint8_t *lock_and_get_surface_ptr();
    int unlock_surface() const;
    int clear();
    SDL_Texture *get_texture();
    int copy_surface_to_buffer(uint8_t *buffer_ptr);
    int get_surface_width() const;
    int get_surface_height() const;
    const SDL_Rect *get_texture_coord() const;
    int set_render_destination(int x, int y, int w, int h);
    const SDL_Rect *get_render_destination() const;
    int set_render_source(int x, int y, int w, int h);
    const SDL_Rect *get_render_source() const;

private:
    SDL_Surface *m_surface = nullptr;
    bool m_must_lock_surface = false;
    Texture m_texture;
    SDL_Rect m_texture_coord_in_surface = { 0 };
    SDL_Rect m_render_source = { 0 };
    SDL_Rect m_render_destination = { 0 };
};
