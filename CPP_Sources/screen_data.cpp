#include <iostream>

#include "bedlam2_draw.h"
#include "screen_data.h"
#include "palette.h"

Screen_data::Screen_data()
{
}

Screen_data::Screen_data(uint8_t *screen_buffer, int surface_width, int surface_height, int offset_x, int offset_y)
{
    init(screen_buffer, surface_width, surface_height, offset_x, offset_y);
}

Screen_data::~Screen_data()
{
    destroy();
}

int Screen_data::destroy()
{
    if (m_surface)
    {
        SDL_FreeSurface(m_surface);
        m_surface = nullptr;
    }
    return m_texture.destroy();
}

int Screen_data::init(uint8_t *screen_buffer, int surface_width, int surface_height, int img_width,
    int img_height, int offset_x, int offset_y)
{
    int ret_val = 0;
    ret_val |= destroy();
    m_surface = SDL_CreateRGBSurfaceWithFormatFrom(
        screen_buffer, surface_width, surface_height, 8, surface_width, SDL_PIXELFORMAT_INDEX8);
    if (!m_surface)
    {
        std::cout << "ERROR: created screen surface. " << SDL_GetError() << std::endl;
        return -1;
    }
    m_must_lock_surface = SDL_MUSTLOCK(m_surface);

    if (surface_width - offset_x < 0 || surface_height - offset_y < 0)
    {
        std::cout << "ERROR: screen data invalid surface and offset params." << std::endl;
        ret_val |= -1;
    }
    m_texture_coord_in_surface.x = offset_x;
    m_texture_coord_in_surface.y = offset_y;
    m_texture_coord_in_surface.w = img_width;
    m_texture_coord_in_surface.h = img_height;
    ret_val |= m_texture.create_texture_from_surface(m_surface, &m_texture_coord_in_surface);
    return ret_val;
}

int Screen_data::set_palette(const uint8_t *pal_ptr, int offset, int num_entries)
{
    int ret_val = 0;
    SDL_Color color;
    if (m_must_lock_surface)
    {
        ret_val |= SDL_LockSurface(m_surface);
    }
    SDL_Palette *p = m_surface->format->palette;
    if (!p)
    {
        ret_val |= 1;
    }
    int n = 0;
    for (int i = offset; i < num_entries; i++)
    {
        color.r = 4 * pal_ptr[n * 3];
        color.g = 4 * pal_ptr[n * 3 + 1];
        color.b = 4 * pal_ptr[n * 3 + 2];
        color.a = 0;
        n++;

        ret_val |= SDL_SetPaletteColors(p, &color, i, 1);
    }
    ret_val |= SDL_SetSurfacePalette(m_surface, p);
    if (ret_val)
    {
        std::cout << "ERROR: set screen palette \n";
    }
    if (m_must_lock_surface)
    {
        SDL_UnlockSurface(m_surface);
    }
    return ret_val;
}

int Screen_data::set_palette(const File &palette_file)
{
    const uint8_t *palette_ptr = palette_file.get_ptr();
    int16_t offset = palette_ptr[0];
    int16_t num_entries = palette_ptr[1];
    palette_ptr = palette_ptr + 2;
    if (!num_entries)
    {
        num_entries = 256;
    }
    return set_palette(palette_ptr, offset, num_entries);
}

uint8_t *Screen_data::get_RGB_palette_ptr()
{
    static uint8_t palette[768];
    int32_t i;
    int32_t color_offset; // ecx
    if (m_must_lock_surface)
    {
        SDL_LockSurface(m_surface);
    }
    for (i = 0; i < 256; ++i)
    {
        color_offset = 3 * i;
        palette[color_offset + 0] = m_surface->format->palette->colors[i].r >> 2;
        palette[color_offset + 1] = m_surface->format->palette->colors[i].g >> 2;
        palette[color_offset + 2] = m_surface->format->palette->colors[i].b >> 2;
    }
    if (m_must_lock_surface)
    {
        SDL_UnlockSurface(m_surface);
    }
    return palette;
}

int Screen_data::fill_screen_surface(uint8_t *buffer_ptr)
{
    uint8_t *destin;
    int ret_val = 0;
    if (m_must_lock_surface)
    {
        ret_val |= SDL_LockSurface(m_surface);
    }
    destin = (uint8_t *)m_surface->pixels;
    for (int y = 0; y < ORIGINAL_GAME_HEIGHT; y++)
    {
        memcpy(destin, buffer_ptr, ORIGINAL_GAME_WIDTH);
        destin += m_surface->w;
        buffer_ptr += ORIGINAL_GAME_WIDTH;
    }
    if (m_must_lock_surface)
    {
        SDL_UnlockSurface(m_surface);
    }
    return ret_val;
}

int Screen_data::fill_screen_surface(
    uint8_t *buffer_ptr, int screen_x, int screen_y, int buffer_x, int buffer_y, int buffer_count_w, int buffer_count_h,
    int buffer_w)
{
    uint8_t *destin;
    int ret_val = 0;
    if (m_must_lock_surface)
    {
        ret_val |= SDL_LockSurface(m_surface);
    }
    destin = (uint8_t *)m_surface->pixels;
    if (buffer_y + buffer_count_h > m_surface->h)
    {
        buffer_count_h = m_surface->h - buffer_y;
    }
    if (screen_x + buffer_count_w > m_surface->w)
    {
        buffer_count_w = m_surface->w - screen_x;
    }
    for (int y = 0; y < buffer_count_h; y++)
    {
        memcpy(
            &destin[screen_x + (y + screen_y) * m_surface->w], &buffer_ptr[buffer_x + (y + buffer_y) * buffer_w],
            buffer_count_w);
    }
    if (m_must_lock_surface)
    {
        SDL_UnlockSurface(m_surface);
    }
    return ret_val;
}

int Screen_data::update_texture()
{
    return m_texture.fill_texture_from_surface(m_surface, &m_texture_coord_in_surface);
}

uint8_t *Screen_data::lock_and_get_surface_ptr()
{
    if (m_must_lock_surface)
    {
        SDL_LockSurface(m_surface);
    }
    return (uint8_t *)m_surface->pixels;
}

int Screen_data::unlock_surface() const
{
    if (m_must_lock_surface)
    {
        SDL_UnlockSurface(m_surface);
    }
    return 0;
}

int Screen_data::clear()
{
    int ret_val = 0;
    if (m_must_lock_surface)
    {
        ret_val |= SDL_LockSurface(m_surface);
    }
    memset(m_surface->pixels, 0, m_surface->w * m_surface->h);
    if (m_must_lock_surface)
    {
        SDL_UnlockSurface(m_surface);
    }
    return ret_val;
}

SDL_Texture *Screen_data::get_texture()
{
    update_texture();
    return m_texture.get();
}

int Screen_data::copy_surface_to_buffer(uint8_t *buffer_ptr)
{
    int ret_val = 0;
    if (m_must_lock_surface)
    {
        ret_val |= SDL_LockSurface(m_surface);
    }
    uint8_t *source = (uint8_t *)m_surface->pixels;
    for (int y = 0; y < ORIGINAL_GAME_HEIGHT; y++)
    {
        memcpy(buffer_ptr, source, ORIGINAL_GAME_WIDTH);
        buffer_ptr += ORIGINAL_GAME_WIDTH;
        source += m_surface->w;
    }
    if (m_must_lock_surface)
    {
        SDL_UnlockSurface(m_surface);
    }
    return ret_val;
}

int Screen_data::get_surface_width() const
{
    return m_surface->w;
}

int Screen_data::get_surface_height() const
{
    return m_surface->h;
}

const SDL_Rect *Screen_data::get_texture_coord() const
{
    return &m_texture_coord_in_surface;
}

int Screen_data::set_render_destination(int x, int y, int w, int h)
{
    m_render_destination.x = x;
    m_render_destination.y = y;
    m_render_destination.w = w;
    m_render_destination.h = h;
    return 0;
}

const SDL_Rect* Screen_data::get_render_destination() const
{
    return &m_render_destination;
}

int Screen_data::set_render_source(int x, int y, int w, int h)
{
    m_render_source.x = x;
    m_render_source.y = y;
    m_render_source.w = w;
    m_render_source.h = h;
    return 0;
}
const SDL_Rect* Screen_data::get_render_source() const
{
    return &m_render_source;
}