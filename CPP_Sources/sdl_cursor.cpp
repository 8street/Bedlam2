#include <iostream>

#include "exported_func.h"
#include "file.h"
#include "mouse.h"
#include "sdl_cursor.h"

cursor::cursor()
{
}

cursor::~cursor()
{
    SDL_FreeCursor(m_crosshair);
    SDL_FreeCursor(m_default);
    for (SDL_Cursor *cur : m_waiting_cursor)
    {
        SDL_FreeCursor(cur);
    }
}

int cursor::init()
{
    int ret_val = 0;
    BIN_File general_bin("GAMEGFX/GENERAL.BIN");
    File gamepal_pal("GAMEGFX/GAMEPAL.PAL");

    ret_val |= load_cursors(general_bin, gamepal_pal);
    ret_val |= set_waiting_cursor();
    show_cursor();
    if (ret_val)
    {
        std::cout << "ERROR: Init cursor." << std::endl;
    }
    return ret_val;
}

int cursor::hide_cursor()
{
    return SDL_ShowCursor(SDL_DISABLE);
}

int cursor::show_cursor()
{
    return SDL_ShowCursor(SDL_ENABLE);
}

int cursor::set_cursor_icon(int icon)
{
    switch (icon)
    {
        case ICON_CURSOR:
            return set_default_cursor();
        case ICON_CROSSHAIR:
            return set_crosshair_cursor();
        case ICON_WAIT:
            return set_waiting_cursor();
        default:
            return update_waiting_cursor();
            break;
    }
}

int cursor::load_cursors(const BIN_File &general_bin, const File &pal_file)
{
    int ret_val = 0;
    int img = 0;

    // first in pal file is offset and num entries bytes, see palette.cpp
    const uint8_t *pal_ptr = pal_file.get_ptr() + 2;

    // load default cursor
    img = 93;
    Texture default_cursor_texture(img, general_bin, pal_file);
    m_default = load_cursor(default_cursor_texture);

    // load crosshair cursor
    img = 0;
    Texture crosshair_texture(img, general_bin, pal_file);
    m_crosshair = load_cursor(crosshair_texture);

    // load waiting cursor
    for (img = ICON_WAIT; img < 152; img++)
    {
        Texture waiting_cursor_texture(img, general_bin, pal_file);
        SDL_Cursor *c = load_cursor(waiting_cursor_texture);
        m_waiting_cursor.push_back(c);
        if (c == NULL)
        {
            ret_val |= 1;
        }
    }

    if (m_default == NULL || m_crosshair == NULL || ret_val)
    {
        ret_val |= 1;
    }
    return ret_val;
}

int cursor::set_waiting_cursor()
{
    SDL_SetCursor(m_waiting_cursor.at(0));
    m_cursor_is_waiting = true;
    m_img = ICON_WAIT;
    return 0;
}

int cursor::update_waiting_cursor()
{
    if (m_cursor_is_waiting)
    {
        m_waiting_timer++;
        if (m_waiting_timer >= static_cast<int>(m_waiting_cursor.size()))
        {
            m_waiting_timer = 0;
        }
        SDL_SetCursor(m_waiting_cursor.at(m_waiting_timer));
    }
    return 0;
}

int cursor::set_crosshair_cursor()
{
    SDL_SetCursor(m_crosshair);
    m_cursor_is_waiting = false;
    m_img = ICON_CROSSHAIR;
    return 0;
}

int cursor::set_default_cursor()
{
    SDL_SetCursor(m_default);
    m_cursor_is_waiting = false;
    m_img = ICON_CURSOR;
    return 0;
}
int cursor::get_cursor_img() const
{
    return m_img;
}

SDL_Cursor *cursor::load_cursor(const Texture &cursor_texture)
{
    int ret_val = 0;
    SDL_Surface *cursor_surface = nullptr;
    SDL_Cursor *cursor = nullptr;

    ret_val |= SDL_LockTextureToSurface(cursor_texture.get(), NULL, &cursor_surface);
    cursor = SDL_CreateColorCursor(cursor_surface, 9, 9);
    SDL_UnlockTexture(cursor_texture.get());

    if (ret_val || cursor == NULL)
    {
        std::cout << "ERROR: create cursor. " << SDL_GetError() << std::endl;
    }

    return cursor;
}