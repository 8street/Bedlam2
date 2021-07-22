#include <iostream>

#include "bedlam2.h"
#include "sdl_draw.h"
#include "sdl_event.h"
#include "ddraw_func.h"
#include "sdl_timer.h"
#include "timer.h"

SDL_Window *WINDOW;
SDL_Renderer *RENDER;
SDL_Surface *SCREEN_SURFACE;

#define SIDEBAR_WIDTH 160

uint8_t *SCREEN_BUFFER_PTR;
uint8_t GAME_SCREEN_PTR[409600];


int init_video()
{

    int ret_val = 0;
    ret_val |= SDL_Init(SDL_INIT_VIDEO);
    int window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN;
    WINDOW = SDL_CreateWindow(
        "Bedlam 2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GAME_WIDTH, GAME_HEIGHT, window_flags);
    RENDER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    if (RENDER == NULL)
    {
        std::cout << "ERROR: created accelerated renderer is invalid. \n";
        RENDER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_TARGETTEXTURE);
        if (RENDER == NULL)
        {
            std::cout << "ERROR: created software renderer is invalid. \n";
            ret_val |= 1;
        }
    }

    SDL_SetWindowMinimumSize(WINDOW, GAME_WIDTH, GAME_HEIGHT);

    ret_val |= SDL_SetRenderTarget(RENDER, NULL);
    ret_val |= SDL_SetRenderDrawBlendMode(RENDER, SDL_BLENDMODE_BLEND);

    ret_val |= SDL_RenderClear(RENDER);
    SDL_RenderPresent(RENDER);
    SCREEN_BUFFER_PTR = new uint8_t[GAME_WIDTH * GAME_HEIGHT];
    memset(SCREEN_BUFFER_PTR, 0, GAME_WIDTH * GAME_HEIGHT);

    SCREEN_SURFACE_WIDTH = GAME_WIDTH;
    SCREEN_SURFACE_HEIGHT = GAME_HEIGHT;

    SCREEN_SURFACE = SDL_CreateRGBSurfaceWithFormatFrom(
        SCREEN_BUFFER_PTR, GAME_WIDTH, GAME_HEIGHT, 8, GAME_WIDTH, SDL_PIXELFORMAT_INDEX8);
    if (!SCREEN_SURFACE)
    {
        std::cout << "ERROR: created screen surface. \n";
        ret_val |= 1;
    }

    if (ret_val)
    {
        std::cout << "ERROR: init window \n";
    }

    return ret_val;
}

int sdl_set_palette(uint8_t *pal_ptr, int offset, int num_entries)
{
    int ret_val = 0;
    SDL_Color color;
    if (SDL_MUSTLOCK(SCREEN_SURFACE))
    {
        SDL_LockSurface(SCREEN_SURFACE);
    }
    SDL_Palette *p = SCREEN_SURFACE->format->palette;
    if (!p)
    {
        ret_val = 1;
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
    ret_val |= SDL_SetSurfacePalette(SCREEN_SURFACE, p);
    if (ret_val)
    {
        std::cout << "ERROR: set screen palette \n";
    }
    if (SDL_MUSTLOCK(SCREEN_SURFACE))
    {
        SDL_UnlockSurface(SCREEN_SURFACE);
    }
    //redraw();
    return ret_val;
}

// 0042597C Bedlam 1
void clear_screen()
{
    if (SDL_MUSTLOCK(SCREEN_SURFACE))
    {
        SDL_LockSurface(SCREEN_SURFACE);
    }
    memset(SCREEN_SURFACE->pixels, 0, GAME_WIDTH * GAME_HEIGHT);
    //memset(SCREEN_BUFFER_PTR, 0, GAME_WIDTH * GAME_HEIGHT);
    if (SDL_MUSTLOCK(SCREEN_SURFACE))
    {
        SDL_UnlockSurface(SCREEN_SURFACE);
    }
    redraw();
}

// 00425A1E Bedlam 1
void copy_buffer_to_screen_and_unlock(uint8_t *buffer)
{
    //int i;           // [esp+0h] [ebp-20h]
    uint8_t *destin; // [esp+4h] [ebp-1Ch]

    if (SDL_MUSTLOCK(SCREEN_SURFACE))
    {
        SDL_LockSurface(SCREEN_SURFACE);
    }
    destin = (uint8_t *)SCREEN_SURFACE->pixels;
    memcpy(destin, buffer, GAME_WIDTH * GAME_HEIGHT);
    if (SDL_MUSTLOCK(SCREEN_SURFACE))
    {
        SDL_UnlockSurface(SCREEN_SURFACE);
    }
}

// 00425A03 Bedlam 1
void redraw()
{
    //Timer tim;
    SDL_Texture *t = MY_CreateTextureFromSurface(RENDER, SCREEN_SURFACE);
    SDL_RenderCopy(RENDER, t, NULL, NULL);
    //volatile double a = tim.elapsed();
    //a = 0.0;
    SDL_RenderPresent(RENDER);
    SDL_DestroyTexture(t);
    SDL_events();
}

// 0041E215 Bedlam 1
int unlock_surface_and_wait(int time_to_waiting)
{
    if (SDL_MUSTLOCK(SCREEN_SURFACE))
    {
        SDL_UnlockSurface(SCREEN_SURFACE);
    }
    WAITING_TIMER = 0;
    redraw();
    while (time_to_waiting > WAITING_TIMER)
    {
        SDL_events();
    }
    return 0;
}


// 0001256B Bedlam 1
void redraw_copy_buff_to_screen_and_unlock(uint8_t *screen_buf)
{
    copy_buffer_to_screen_and_unlock(screen_buf);
}

// 0040129E Bedlam 1
int clear_game_viewport()
{
    uint8_t *destination; // edi
    int line;             // edx

    if (SDL_MUSTLOCK(SCREEN_SURFACE))
    {
        SDL_LockSurface(SCREEN_SURFACE);
    }
    destination = SCREEN_BUFFER_PTR;
    line = SCREEN_SURFACE_HEIGHT;
    do
    {
        memset(destination, 0, GAME_WIDTH - SIDEBAR_WIDTH);
        destination += SCREEN_SURFACE_WIDTH;
        --line;
    } while (line);
    if (SDL_MUSTLOCK(SCREEN_SURFACE))
    {
        SDL_UnlockSurface(SCREEN_SURFACE);
    }
    return 0;
}

// 00401107 Bedlam 1
// Copy map or game screen in SCREEN_BUFFER_PTR
void draw_map_and_game_screen()
{
    uint8_t *source_ptr;
    uint8_t *destination_ptr;
    if (dead_screen_scale)
    {
        dead_screen_scaler();
    }
    else
    {
        if (map_active)
        {
            source_ptr = game_screen_ptr;
        }
        else
        {
            source_ptr = &game_screen_ptr
                             [GAME_WIDTH * (((screen_y_pos & 31) + (screen_x_pos & 31u)) >> 1) // y
                              + 64 * GAME_WIDTH                                                // y offset (64 is tile size)
                              + 64                                                             // x offset
                              + (((screen_x_pos & 31) - (screen_y_pos & 31) + 32) & 63)];      // x
        }
        destination_ptr = SCREEN_BUFFER_PTR;
        int screen_line = GAME_HEIGHT;
        do
        {
            memcpy(destination_ptr, source_ptr, GAME_WIDTH - SIDEBAR_WIDTH);
            source_ptr += GAME_WIDTH;
            destination_ptr += SCREEN_SURFACE_WIDTH;
            --screen_line;
        } while (screen_line);
    }
}

// 004012F7 Bedlam 1
void dead_screen_scaler()
{
    clear_game_viewport();
    uint8_t *source_ptr = &game_screen_ptr
                              [GAME_WIDTH * (((screen_y_pos & 31) + (screen_x_pos & 31u)) >> 1) // y
                               + 64 * GAME_WIDTH                                                // y offset (64 is tile size)
                               + 64                                                             // x offset
                               + (((screen_x_pos & 31) - (screen_y_pos & 31) + 32) & 63)];      // x
    int scaled_screen_width = GAME_WIDTH - dead_screen_scale - SIDEBAR_WIDTH;
    int scaled_screen_height = GAME_HEIGHT - dead_screen_scale;
    uint8_t *destination_ptr = SCREEN_BUFFER_PTR;

    for (int y = 0; y < scaled_screen_height; y++)
    {
        for (int x = 0; x < scaled_screen_width; x++)
        {
            destination_ptr
                [x                                     // x
                 + y * GAME_WIDTH                      // y
                 + dead_screen_scale / 2               // offset x
                 + dead_screen_scale / 2 * GAME_WIDTH] // offset y
                = source_ptr
                    [x * (GAME_WIDTH - SIDEBAR_WIDTH) / scaled_screen_height +       // x
                     y * GAME_HEIGHT / scaled_screen_height * SCREEN_SURFACE_WIDTH]; // y
        }
    }
}

// 000124D1 Bedlam 2
void copy_screen_to_buffer(uint8_t *buffer_ptr)
{
    if (SDL_MUSTLOCK(SCREEN_SURFACE))
    {
        SDL_LockSurface(SCREEN_SURFACE);
    }
    memcpy(buffer_ptr, SCREEN_BUFFER_PTR, SCREEN_SURFACE_WIDTH * SCREEN_SURFACE_HEIGHT);
    if (SDL_MUSTLOCK(SCREEN_SURFACE))
    {
        SDL_UnlockSurface(SCREEN_SURFACE);
    }
}

SDL_Texture *MY_CreateTextureFromSurface(SDL_Renderer *renderer, const SDL_Surface *surface)
{
    int errorcode = 0;
    SDL_Texture *t = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, surface->w, surface->h);
    if (!t)
    {
        errorcode |= 1;
    }
    uint8_t *bytes = nullptr;
    int pitch = 0;
    errorcode |= SDL_LockTexture(t, nullptr, reinterpret_cast<void **>(&bytes), &pitch);
    if (SDL_MUSTLOCK(SCREEN_SURFACE))
    {
        SDL_LockSurface(SCREEN_SURFACE);
    }
    for (int y = 0; y < surface->h; y++)
    {
        for (int x = 0; x < surface->w; x++)
        {
            uint8_t index = ((uint8_t *)surface->pixels)[y * surface->w + x];
            ((SDL_Color *)bytes)[y * surface->w + x] = surface->format->palette->colors[index];
        }
    }
    if (SDL_MUSTLOCK(SCREEN_SURFACE))
    {
        SDL_UnlockSurface(SCREEN_SURFACE);
    }
    SDL_UnlockTexture(t);
    if (errorcode)
    {
        std::cout << "ERROR: creating texture \n";
    }
    return t;
}

