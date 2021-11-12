#pragma once
#include <SDL.h>

class Render_position
{
public:
    Render_position();
    ~Render_position();
    const SDL_Rect *get_render_source() const;
    const SDL_Rect *get_render_destination() const;
    int set_render_source(int x, int y, int w, int h);
    int set_render_destination(int x, int y, int w, int h);

private:
    SDL_Rect m_render_source = { 0 };
    SDL_Rect m_render_destination = { 0 };
};
