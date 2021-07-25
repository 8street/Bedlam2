#include <iostream>

#include "sdl_texture.h"
#include "sdl_window.h"
#include "exported_func.h"

texture::texture()
{
}

texture::texture(int img, const BIN_File &bin)
{
    create_texture(img, bin);
}

texture::texture(int img, const BIN_File &bin, const File &pal)
{
    create_texture(img, bin, pal);
}

texture::texture(SDL_Texture *texture, int width, int height)
    : width(width)
    , height(height)
{
    m_texture = texture;
    offset_x = 0;
    offset_y = 0;
}

texture::~texture()
{
    destroy();
}

texture::texture(const texture &t)
    : width(t.width)
    , height(t.height)
    , offset_x(t.offset_x)
    , offset_y(t.offset_y)
{
    int error_val = 0;
    if (t.m_texture)
    {
        destroy();

        m_texture = SDL_CreateTexture(GAME_WINDOW.get_renderer(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, t.width, t.height);
        if (m_texture == NULL)
        {
            std::cout << "ERROR: create SDL texture in copy constructor. " << SDL_GetError() << ".\n";
        }

        uint8_t *dest_arr = nullptr;
        uint8_t *source_arr = nullptr;
        int dest_pitch = 0;
        int source_pitch = 0;

        error_val |= SDL_LockTexture(m_texture, nullptr, reinterpret_cast<void **>(&dest_arr), &dest_pitch);
        error_val |= SDL_LockTexture(t.m_texture, nullptr, reinterpret_cast<void **>(&source_arr), &source_pitch);

        int dest_width = t.width;
        int dest_height = t.height;

        if (error_val)
        {
            std::cout << "ERROR: texture copy initialization. " << SDL_GetError() << ".\n";
        }
        else
        {
            memcpy(dest_arr, source_arr, source_pitch * t.height);
        }
        SDL_UnlockTexture(t.m_texture);
        SDL_UnlockTexture(m_texture);

        error_val |= SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
        if (error_val)
        {
            std::cout << "ERROR: texture copy initialization. " << SDL_GetError() << ".\n";
        }
    }
    else
    {
        m_texture = nullptr;
    }
}

texture::texture(int width, int height)
    : width(width)
    , height(height)
    , offset_x(0)
    , offset_y(0)
{
    m_texture = SDL_CreateTexture(
        GAME_WINDOW.get_renderer(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
    int error_val = SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
    if (error_val || m_texture == NULL)
    {
        std::cout << "ERROR: create SDL texture. " << SDL_GetError() << ".\n";
    }
}

texture &texture::operator=(const texture &t)
{
    int error_val = 0;
    if (this == &t)
    {
        return *this;
    }

    if (t.m_texture)
    {
        uint8_t *dest_arr = nullptr;
        uint8_t *source_arr = nullptr;
        int dest_pitch = 0;
        int source_pitch = 0;

        destroy();

        m_texture = SDL_CreateTexture(
            GAME_WINDOW.get_renderer(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, t.width, t.height);

        error_val |= SDL_LockTexture(m_texture, nullptr, reinterpret_cast<void **>(&dest_arr), &dest_pitch);
        error_val |= SDL_LockTexture(t.m_texture, nullptr, reinterpret_cast<void **>(&source_arr), &source_pitch);

        int dest_width = t.width;
        int dest_height = t.height;

        if (error_val || m_texture == NULL)
        {
            std::cout << "ERROR: operator=. " << SDL_GetError() << ".\n";
        }
        else
        {
            memcpy(dest_arr, source_arr, source_pitch * t.height);
        }
        SDL_UnlockTexture(t.m_texture);
        SDL_UnlockTexture(m_texture);

        error_val |= SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
        if (error_val)
        {
            std::cout << "ERROR: texture operator=. " << SDL_GetError() << ".\n";
        }
        width = t.width;
        height = t.height;
        offset_x = t.offset_x;
        offset_y = t.offset_y;
    }
    else
    {
        m_texture = nullptr;
        width = 0;
        height = 0;
        offset_x = 0;
        offset_y = 0;
    }

    return *this;
}

int texture::create_texture(int img, const BIN_File &bin)
{
    const File pal;
    return create_texture(img, bin, pal);
}

int texture::create_texture(int img, const BIN_File &bin, const File &pal)
{
    // Fill texture in buffer via standard bedlam function,
    // then create texture from buffer with palette colors
    if (m_texture)
    {
        destroy();
    }
    int ret_val = 0;
    width = bin.get_img_width(img);
    height = bin.get_img_height(img);

    offset_x = bin.get_img_x_offset(img);
    offset_y = bin.get_img_y_offset(img);

    std::vector<uint8_t> buffer(640 * 480, 0);

    const uint16_t img_header = *(uint16_t *)bin.get_img_header_ptr(img);
    const uint16_t bedlam_tile_code = 0x0007;
    if (img_header == bedlam_tile_code)
    {
        if (width && height)
        {
            draw_tile(img, 0, 0, buffer.data(), bin.get_ptr());
        }
        else
        {
            width = 64;
            height = 32;
        }
    }
    else
    {
        if (width && height)
        {
            draw_IMG_in_buffer(img, 1, 0, 0, buffer.data(), bin.get_ptr());
        }
        else
        {
            // create empty texture
            width = 1;
            height = 1;
        }
    }

    m_texture = SDL_CreateTexture(
        GAME_WINDOW.get_renderer(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (m_texture == NULL)
    {
        ret_val |= 1;
        std::cout << "ERROR: create SDL texture. " << SDL_GetError() << ". " << bin.get_full_path() << " Img: " << img << "\n";
    }
    else
    {
        ret_val |= SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);

        const uint8_t *palette = pal.get_ptr() + 2;
        uint8_t *bytes = nullptr;
        int pitch = 0;
        ret_val |= SDL_LockTexture(m_texture, nullptr, reinterpret_cast<void **>(&bytes), &pitch);
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int index = buffer[(offset_y + y) * 640 + x + offset_x];
                bytes[y * pitch + x * 4 + 0] = 4 * palette[index * 3 + 0];
                bytes[y * pitch + x * 4 + 1] = 4 * palette[index * 3 + 1];
                bytes[y * pitch + x * 4 + 2] = 4 * palette[index * 3 + 2];
                bytes[y * pitch + x * 4 + 3] = index ? 255 : 0;
            }
        }
        SDL_UnlockTexture(m_texture);
    }
    return ret_val;
}

int texture::draw(int x, int y) const
{
    int ret_val = 0;
    SDL_Rect destination = { offset_x + x, offset_y + y, width, height };
    ret_val |= SDL_RenderCopy(GAME_WINDOW.get_renderer(), m_texture, NULL, &destination);
    return ret_val;
}

// Not tested in this project
int texture::stretch_draw(int x, int y) const
{
    int ret_val = 0;
    int w = GAME_WINDOW.get_game_width();
    int h = GAME_WINDOW.get_game_height();
    if (w * 3 > h * 4)
    {
        w = w * 4 / 3;
    }
    else
    {
        h = h * 3 / 4;
    }
    SDL_Rect destination = { (offset_x + x) * w / 640, (offset_y + y) * h / 480, width * w / 640, height * h / 480 };
    ret_val |= SDL_RenderCopy(GAME_WINDOW.get_renderer(), m_texture, NULL, &destination);
    return ret_val;
}

int texture::merge_with(const texture &t, int x, int y)
{
    int ret_val = 0;

    uint8_t *dest_arr = nullptr;
    uint8_t *source_arr = nullptr;
    int dest_pitch = 0;
    int source_pitch = 0;

    ret_val |= SDL_LockTexture(m_texture, nullptr, reinterpret_cast<void **>(&dest_arr), &dest_pitch);
    ret_val |= SDL_LockTexture(t.m_texture, nullptr, reinterpret_cast<void **>(&source_arr), &source_pitch);

    int i = 0;
    uint8_t alpha = 0;
    int dest_width = x + t.width;
    int dest_height = y + t.height;
    if (dest_width > width)
    {
        dest_width = width;
    }
    if (dest_height > height)
    {
        dest_height = height;
    }

    for (int h = y; h < dest_height; h++)
    {
        for (int w = x; w < dest_width; w++)
        {
            alpha = source_arr[i + 3];
            if (alpha)
            {
                dest_arr[h * dest_pitch + w * 4 + 0] = source_arr[i + 0];
                dest_arr[h * dest_pitch + w * 4 + 1] = source_arr[i + 1];
                dest_arr[h * dest_pitch + w * 4 + 2] = source_arr[i + 2];
                dest_arr[h * dest_pitch + w * 4 + 3] = alpha;
            }
            i += 4;
        }
    }

    SDL_UnlockTexture(t.m_texture);
    SDL_UnlockTexture(m_texture);

    if (ret_val)
    {
        std::cout << "ERROR: texture merge. " << SDL_GetError() << ".\n";
    }
    return ret_val;
}

int texture::destroy()
{
    if (m_texture)
    {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
    return 0;
}

SDL_Texture *texture::get()
{
    return m_texture;
}

SDL_Texture *const texture::get() const
{
    return m_texture;
}