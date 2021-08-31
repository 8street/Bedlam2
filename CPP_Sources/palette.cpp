#include "palette.h"
#include "helper.h"
#include "sdl_event.h"
#include "sdl_window.h"

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
    GAME_WINDOW.set_palette(pal_ptr, offset, num_entries);
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
        color = *(uint16_t *)&ANIMATE_PALETTE_BUFER[buf_ofst + 2] + *(uint16_t *)&ANIMATE_PALETTE_BUFER[buf_ofst];
        *(uint16_t *)&ANIMATE_PALETTE_BUFER[buf_ofst] = color;
        ANIMATE_PALETTE_PTR[ofst++] = (color >> 8) & 0xFF;
        buf_ofst += 4;
    } while (ofst < 768);
    NEW_PALETTE = ANIMATE_PALETTE_PTR;
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
    screen_palette_ptr = GAME_WINDOW.get_RGB_palette_ptr();
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
        *(uint16_t *)buf_file = (uint16_t)screen_color << 8;
        buf_file += 2;
        *(uint16_t *)buf_file = steep;
        buf_file += 2;
        screen_palette_ptr++;
    }
    PALETTE_TIMER = time;
}

// 0041FA3F Bedlam 1
void wait_palette_animation(int target_time)
{
    while (PALETTE_TIMER > target_time)
    {
        SDL_events();
    }
}
