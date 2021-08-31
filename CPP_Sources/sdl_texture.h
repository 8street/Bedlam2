#pragma once
#include <SDL.h>

#include "bin_file.h"

class Texture
{
public:
    Texture();
    Texture(int img, const BIN_File &bin);
    Texture(int img, const BIN_File &bin, const File &pal);
    Texture(SDL_Texture *texture, int width, int height);
    Texture(const Texture &t);
    Texture(int width, int height);
    ~Texture();
    Texture &operator=(const Texture &t);

    int create_texture(int img, const BIN_File &bin);
    int create_texture(int img, const BIN_File &bin, const File &pal);
    int create_texture_from_surface(SDL_Surface *surface, SDL_Rect *texture_rect);
    int fill_texture_from_surface(SDL_Surface *surface, SDL_Rect *texture_rect);
    int draw(int x, int y) const;
    int stretch_draw(int x, int y) const;
    int merge_with(const Texture &t, int x, int y);

    int destroy();
    SDL_Texture *get();
    SDL_Texture *const get() const;
    SDL_Rect *const get_rect() const;

    int width = 0;
    int height = 0;

    int m_offset_x = 0;
    int m_offset_y = 0;

private:
    SDL_Texture *m_texture = nullptr;
};