#include "palette.h"
#include "ddraw_func.h"
#include "sdl_draw.h"
#include "sdl_event.h"

volatile int32_t PALETTE_TIMER;
uint8_t ANIMATE_PALETTE_PTR[780];
uint8_t ANIMATE_PALETTE_BUFER[3082];
volatile uint8_t *NEW_PALETTE;

// 004258D0 Bedlam 1
void set_palette(uint8_t *pal_file)
{
    int16_t offset;      // si
    int16_t num_entries; // dx
    uint8_t *pal_ptr;    // ecx

    offset = pal_file[0];
    num_entries = pal_file[1];
    pal_ptr = pal_file + 2;
    if (!num_entries)
    {
        num_entries = 256;
    }
    sdl_set_palette(pal_ptr, offset, num_entries);
}

// 00425901 Bedlam 1
void palette_animation()
{
    int ofst;      // edx
    int buf_ofst;  // ecx
    int16_t color; // bx

    ofst = 0;
    buf_ofst = 0;
    do
    {
        // color = steep + old_color
        color = *(WORD *)&ANIMATE_PALETTE_BUFER[buf_ofst + 2] + *(WORD *)&ANIMATE_PALETTE_BUFER[buf_ofst];
        *(WORD *)&ANIMATE_PALETTE_BUFER[buf_ofst] = color;
        ANIMATE_PALETTE_PTR[ofst++] = HIBYTE(color);
        buf_ofst += 4;
    } while (ofst < 768);
    NEW_PALETTE = ANIMATE_PALETTE_PTR;
    //sdl_set_palette(ANIMATE_PALETTE_PTR, 0, 256);
}

// 0041CBF0 Bedlam 1
void swap_palette_with_animation(uint8_t *palette_file_ptr, int time)
{
    uint8_t *palette_ptr;        // edi
    uint8_t *buf_file;           // ebx
    uint8_t *screen_palette_ptr; // ecx
    int screen_color;            // esi
    int in_color;                // edx
    int steep;                   // eax
    uint8_t *end_pallete;        // [esp+0h] [ebp-1Ch]

    PALETTE_TIMER = 0;
    palette_ptr = palette_file_ptr + 2;
    buf_file = ANIMATE_PALETTE_BUFER;
    screen_palette_ptr = get_RGB_palette_ptr();
    end_pallete = screen_palette_ptr + 768;
    while (screen_palette_ptr != end_pallete)
    {
        screen_color = (uint8_t)*screen_palette_ptr;
        in_color = (uint8_t)*palette_ptr++;
        if (in_color <= screen_color)
        {
            steep = -((((screen_color - in_color) << 8) + 1) / time);
        }
        else
        {
            steep = (((in_color - screen_color) << 8) + 1) / time;
        }
        *(WORD *)buf_file = (WORD)screen_color << 8;
        buf_file += 2;
        *(WORD *)buf_file = steep;
        buf_file += 2;
        screen_palette_ptr++;
    }
    //if (time < 0 || time > 100)
    //{
    //    time = 0;
    //}
    PALETTE_TIMER = time;
}

// 0044B040 Bedlam 1
uint8_t *get_RGB_palette_ptr()
{
    static uint8_t palette[769];
    int32_t i;
    int32_t color_offset; // ecx

    for (i = 0; i < 256; ++i)
    {
        color_offset = 3 * i;
        palette[color_offset + 0] = SCREEN_SURFACE->format->palette->colors[i].r >> 2;
        palette[color_offset + 1] = SCREEN_SURFACE->format->palette->colors[i].g >> 2;
        palette[color_offset + 2] = SCREEN_SURFACE->format->palette->colors[i].b >> 2;
    }
    return palette;
}

// 0041FA3F Bedlam 1
void wait_palette_animation(int target_time)
{
    while (PALETTE_TIMER > target_time)
    {
        SDL_events();
    }
}
