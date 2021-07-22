#pragma once
#include <SDL.h>

#include "bin_file.h"

class texture
{
public:
    texture();
    texture(int img, const BIN_File &bin);
    texture(int img, const BIN_File &bin, const File &pal);
    texture(SDL_Texture *texture, int width, int height);
    texture(const texture &t);
    texture(int width, int height);
    ~texture();
    texture &operator=(const texture &t);

    int create_texture(int img, const BIN_File &bin);
    int create_texture(int img, const BIN_File &bin, const File &pal);
    int draw(int x, int y) const;
    int stretch_draw(int x, int y) const;
    int merge_with(const texture &t, int x, int y);

    int destroy();
    SDL_Texture *get();
    SDL_Texture *const get() const;

    int width = 0;
    int height = 0;

    int offset_x = 0;
    int offset_y = 0;

private:
    SDL_Texture *m_texture = nullptr;
};