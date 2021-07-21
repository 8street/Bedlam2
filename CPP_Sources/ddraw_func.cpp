#include <stdio.h>

#include "bedlam2.h"
#include "ddraw_func.h"
#include "main.h"
#include "mouse.h"
#include "timers.h"
#include "window.h"

#include "sdl_event.h"

int32_t GAME_WIDTH = 640;
int32_t GAME_HEIGHT = 480;

#define SIDEBAR_WIDTH 160

volatile uint8_t FULLSCREEN;
volatile uint8_t SURFACE_IS_LOCKED;
volatile uint8_t DDRAW_CREATED;

volatile uint16_t USE_VIDEOMEMORY;

volatile uint32_t SCREEN_SURFACE_WIDTH;
volatile uint32_t SCREEN_SURFACE_HEIGHT;

LPDIRECTDRAW lpDD;

LPDIRECTDRAWSURFACE DDRAW_SECOND_SCREEN_SURFACE;
LPDIRECTDRAWSURFACE DDRAW_PRIMARY_SCREEN_SURFACE;
LPDIRECTDRAWSURFACE DDRAW_SMACK_SURFACE;
LPDIRECTDRAWSURFACE DDRAW_GAMECURSOR_SURFACE;
LPDIRECTDRAWSURFACE DDRAW_MOUSE_BG_SURFACE;

LPDIRECTDRAWPALETTE DDRAW_SCREEN_PALETTE;
LPDIRECTDRAWCLIPPER DDRAW_SURFACE_CLIPPER;

// TODO: shift buffer to 1 pos left and delete first entry
PALETTEENTRY PALETTEENTRY_BUFFER[258];

volatile uint8_t NEED_PALETTE_UPDATE;

volatile uint8_t IS_BLITTING;




// 0044ACF4 Bedlam 1
HRESULT unlock_surface()
{
    HRESULT result; // eax

    result = SURFACE_IS_LOCKED;
    if (SURFACE_IS_LOCKED)
    {
        result = DDRAW_SECOND_SCREEN_SURFACE->Unlock(NULL);
    }
    return result;
}

// 0044AC5C Bedlam 1
uint8_t *lock_and_get_surface_ptr()
{
    DDSURFACEDESC surface_description; // [esp+20h] [ebp-64h] OVERLAPPED BYREF

    if (!APPLICATION_ACTIVE || !SURFACE_IS_LOCKED)
    {
        return NULL;
    }
    if (DDRAW_PRIMARY_SCREEN_SURFACE->IsLost() == DDERR_SURFACELOST)
    {
        DDRAW_PRIMARY_SCREEN_SURFACE->Restore();
    }
    surface_description.dwSize = sizeof(surface_description);
    if (DDRAW_SECOND_SCREEN_SURFACE->Lock(NULL, &surface_description, DDLOCK_WAIT, NULL) != DD_OK)
    {
        return NULL;
    }
    SCREEN_SURFACE_WIDTH = surface_description.lPitch;
    SCREEN_SURFACE_HEIGHT = surface_description.dwHeight;

    return (uint8_t *)(surface_description.lpSurface);
}

// 0044B7B0 Bedlam 1
int blit_cursor_bg_to_screen()
{
    int result = 0;                // eax
    uint16_t drawing_cursor_pos_x; // bx
    uint16_t drawing_cursor_pos_y; // dx
    tagRECT destination;           // [esp+28h] [ebp-1Ch] BYREF
    tagRECT source;                // [esp+38h] [ebp-Ch] OVERLAPPED BYREF

    if (DDRAW_PRIMARY_SCREEN_SURFACE)
    {
        result = CURSOR_X1;
        if (CURSOR_X1 != -1)
        {
            if (DDRAW_PRIMARY_SCREEN_SURFACE->IsLost() == DDERR_SURFACELOST)
            {
                DDRAW_PRIMARY_SCREEN_SURFACE->Restore();
            }
            if (DDRAW_MOUSE_BG_SURFACE->IsLost() == DDERR_SURFACELOST)
            {
                DDRAW_MOUSE_BG_SURFACE->Restore();
            }
            drawing_cursor_pos_x = WINDOW_POS_X + GAME_WIDTH - CURSOR_X1;
            if (drawing_cursor_pos_x > CURSOR_SURFACE_SIZE)
            {
                drawing_cursor_pos_x = CURSOR_SURFACE_SIZE;
            }
            drawing_cursor_pos_y = WINDOW_POS_Y + GAME_HEIGHT - CURSOR_Y1;
            if (drawing_cursor_pos_y > CURSOR_SURFACE_SIZE)
            {
                drawing_cursor_pos_y = CURSOR_SURFACE_SIZE;
            }
            destination.left = CURSOR_X1;
            destination.top = CURSOR_Y1;
            destination.right = drawing_cursor_pos_x + CURSOR_X1;
            destination.bottom = drawing_cursor_pos_y + CURSOR_Y1;
            source.left = 0;
            source.top = 0;
            source.bottom = drawing_cursor_pos_y;
            source.right = drawing_cursor_pos_x;
            result = DDRAW_PRIMARY_SCREEN_SURFACE->Blt(&destination, DDRAW_MOUSE_BG_SURFACE, &source, 0x1000000, NULL);
            CURSOR_X1 = -1;
        }
    }
    return result;
}

// 0044B6D4 Bedlam 1
void blit_cursor_bg()
{
    if (1)
    {
        blit_cursor_bg_to_screen();
    }
    CURSOR_X1 = -1;
}

// 0044AD18 Bedlam 1
int blit_second_surface_to_screen()
{
    //int result = 0;         // eax
    //tagRECT destination;    // [esp+0h] [ebp-3Ch] BYREF
    //tagRECT source;         // [esp+10h] [ebp-2Ch] BYREF
    //struct tagPOINT Cursor; // [esp+20h] [ebp-1Ch] BYREF
    //int start_time;

    //if (APPLICATION_ACTIVE)
    //{
    //    result = SURFACE_IS_LOCKED;
    //    IS_BLITTING = 1;
    //    if (SURFACE_IS_LOCKED)
    //    {
    //        if (UPDATE_CURSOR_BY_TIMER)
    //        {
    //            start_time = WAITING_TIMER;
    //            while (CURSOR_IS_BLITTING)
    //            {
    //                if (abs(WAITING_TIMER - start_time) > 200)
    //                {
    //                    IS_BLITTING = 0;
    //                    return -1;
    //                }
    //            }
    //            CURSOR_UNKNOWN = 1;
    //            blit_cursor_bg();
    //        }
    //        if (FULLSCREEN)
    //        {
    //            DDRAW_PRIMARY_SCREEN_SURFACE->Flip(NULL, DDFLIP_WAIT);
    //        }
    //        else
    //        {
    //            source.left = 0;
    //            source.top = 0;
    //            source.right = GAME_WIDTH;
    //            source.bottom = GAME_HEIGHT;
    //            destination.left = WINDOW_POS_X;
    //            destination.top = WINDOW_POS_Y;
    //            destination.right = WINDOW_POS_X + GAME_WIDTH;
    //            destination.bottom = WINDOW_POS_Y + GAME_HEIGHT;
    //            DDRAW_PRIMARY_SCREEN_SURFACE->Blt(&destination, DDRAW_SECOND_SCREEN_SURFACE, &source, 0x1000000, NULL);
    //        }
    //        if (UPDATE_CURSOR_BY_TIMER)
    //        {
    //            CURSOR_UNKNOWN = 0;
    //            GetCursorPos(&Cursor);
    //            blit_cursor(Cursor.x, Cursor.y);
    //        }
    //        result = NEED_PALETTE_UPDATE;
    //        if (NEED_PALETTE_UPDATE)
    //        {
    //            result = DDRAW_PRIMARY_SCREEN_SURFACE->SetPalette(DDRAW_SCREEN_PALETTE);
    //        }
    //        NEED_PALETTE_UPDATE = 0;
    //    }
    //    IS_BLITTING = 0;
    //}
    return 0;
}

// 0044B4FC Bedlam 1
int blit_cursor(int x, int y)
{
    int result = 0;      // eax
    __int16 size_x;      // ax
    __int16 size_y;      // dx
    tagRECT cursor_rect; // [esp+30h] [ebp-34h] BYREF
    tagRECT screen_rect; // [esp+40h] [ebp-24h] BYREF

    if (x < WINDOW_POS_X)
    {
        x = WINDOW_POS_X;
    }
    if (x >= WINDOW_POS_X + GAME_WIDTH)
    {
        x = WINDOW_POS_X + GAME_WIDTH;
    }
    if (y < WINDOW_POS_Y)
    {
        y = WINDOW_POS_Y;
    }
    if (y >= WINDOW_POS_Y + GAME_HEIGHT)
        y = WINDOW_POS_Y + GAME_HEIGHT;
    if (DDRAW_PRIMARY_SCREEN_SURFACE)
    {
        result = CURSOR_IS_BLITTING;
        if (!CURSOR_IS_BLITTING)
        {
            result = HIWORD(CURSOR_UNKNOWN);
            if (HIWORD(CURSOR_UNKNOWN) != 1)
            {
                CURSOR_IS_BLITTING = 1;
                if (DDRAW_PRIMARY_SCREEN_SURFACE->IsLost() == DDERR_SURFACELOST)
                {
                    DDRAW_PRIMARY_SCREEN_SURFACE->Restore();
                }
                if (DDRAW_GAMECURSOR_SURFACE->IsLost() == DDERR_SURFACELOST)
                {
                    DDRAW_GAMECURSOR_SURFACE->Restore();
                }
                if (DDRAW_MOUSE_BG_SURFACE->IsLost() == DDERR_SURFACELOST)
                    DDRAW_MOUSE_BG_SURFACE->Restore();
                blit_cursor_bg_to_screen();
                size_x = WINDOW_POS_X + GAME_WIDTH - x;
                if (size_x > CURSOR_SURFACE_SIZE)
                {
                    size_x = CURSOR_SURFACE_SIZE;
                }
                size_y = WINDOW_POS_Y + GAME_HEIGHT - y;
                if (size_y > CURSOR_SURFACE_SIZE)
                {
                    size_y = CURSOR_SURFACE_SIZE;
                }
                screen_rect.left = 0;
                screen_rect.top = 0;
                screen_rect.right = size_x;
                screen_rect.bottom = size_y;
                cursor_rect.left = x;
                cursor_rect.top = y;
                cursor_rect.right = x + size_x;
                cursor_rect.bottom = y + size_y;
                DDRAW_MOUSE_BG_SURFACE->Blt(&screen_rect, DDRAW_PRIMARY_SCREEN_SURFACE, &cursor_rect, 0x1000000, NULL);
                DDRAW_PRIMARY_SCREEN_SURFACE->Blt(&cursor_rect, DDRAW_GAMECURSOR_SURFACE, &screen_rect, 0x1008000, NULL);
                CURSOR_X1 = x;
                CURSOR_Y1 = y;
                result = 0;
                CURSOR_IS_BLITTING = 0;
            }
        }
    }
    return result;
}

// 0044B1C0 Bedlam 1
void activate_app(int32_t activate)
{
    HDC dc; // eax

    if (!FULLSCREEN)
    {
        dc = GetDC(NULL);
        if (dc)
        {
            if ((GetDeviceCaps(dc, RASTERCAPS) & RC_PALETTE) != 0)
            {
                if (activate && GetSystemPaletteUse(dc) == SYSPAL_STATIC)
                {
                    SetSystemPaletteUse(dc, SYSPAL_NOSTATIC);
                }
                else if (!activate)
                {
                    SetSystemPaletteUse(dc, SYSPAL_STATIC);
                }
            }
            ReleaseDC(NULL, dc);
            if (activate)
            {
                if (DDRAW_PRIMARY_SCREEN_SURFACE)
                {
                    DDRAW_PRIMARY_SCREEN_SURFACE->Restore();
                    DDRAW_SCREEN_PALETTE->SetEntries(0, 0, 256, &PALETTEENTRY_BUFFER[1]);
                    DDRAW_PRIMARY_SCREEN_SURFACE->SetPalette(DDRAW_SCREEN_PALETTE);
                }
            }
        }
    }
}

// 00425A8B Bedlam 1
void get_screen_buffer_ptr()
{
    //while (!SCREEN_BUFFER_PTR)
    //{
    //    SCREEN_BUFFER_PTR = lock_and_get_surface_ptr();
    //}
    //if (DDRAW_PRIMARY_SCREEN_SURFACE->IsLost() == DDERR_SURFACELOST)
    //{
    //    DDRAW_PRIMARY_SCREEN_SURFACE->Restore();
    //}
    //if (DDRAW_SECOND_SCREEN_SURFACE->IsLost() == DDERR_SURFACELOST)
    //{
    //    DDRAW_SECOND_SCREEN_SURFACE->Restore();
    //}
}

// 00425AA0 Bedlam 1
HRESULT unlock_surface_and_screen_ptr()
{
    //HRESULT result = DD_OK; // eax

    //if (SCREEN_BUFFER_PTR)
    //{
    //    result = unlock_surface();
    //    SCREEN_BUFFER_PTR = NULL;
    //}
    return 0;
}

// 0044AED4 Bedlam 1
HRESULT ddraw_setpalettes(uint8_t *pal_ptr, int16_t offset, int16_t num_entries)
{
    int32_t entry; // edx
    int32_t ofst;  // ebx
    int32_t first_entry = 0;
    HRESULT ret = DD_OK;

    if (SURFACE_IS_LOCKED)
    {
        first_entry = offset;
        NEED_PALETTE_UPDATE = 1;
        /*       if (!FULLSCREEN) {
                   first_entry = offset - 1;
               }*/
        entry = 0;
        if (num_entries > 0)
        {
            do
            {
                ofst = entry + offset;
                if (ofst >= 0)
                {
                    PALETTEENTRY_BUFFER[ofst + 1].peRed = 4 * pal_ptr[entry * 3 + 0];
                    PALETTEENTRY_BUFFER[ofst + 1].peGreen = 4 * pal_ptr[entry * 3 + 1];
                    PALETTEENTRY_BUFFER[ofst + 1].peBlue = 4 * pal_ptr[entry * 3 + 2];
                }
                entry++;
            } while (entry < num_entries);
        }
        // if (first_entry < 0)
        //{
        //    ++offset;
        //    --num_entries;
        //    ++first_entry;
        //}
        if (DDRAW_PRIMARY_SCREEN_SURFACE->IsLost() == DDERR_SURFACELOST)
        {
            DDRAW_PRIMARY_SCREEN_SURFACE->Restore();
            DDRAW_PRIMARY_SCREEN_SURFACE->SetPalette(DDRAW_SCREEN_PALETTE);
        }
        if (!FULLSCREEN)
        {
            DDRAW_SECOND_SCREEN_SURFACE->Unlock(NULL);
        }
        if ((uint32_t)DDRAW_SCREEN_PALETTE->SetEntries(0, first_entry, num_entries, &PALETTEENTRY_BUFFER[offset + 1]))
        {
            DDRAW_PRIMARY_SCREEN_SURFACE->SetPalette(DDRAW_SCREEN_PALETTE);
            DDRAW_SCREEN_PALETTE->SetEntries(0, first_entry, num_entries, &PALETTEENTRY_BUFFER[offset + 1]);
        }
        ret = DDRAW_PRIMARY_SCREEN_SURFACE->IsLost();
        if (ret == DDERR_SURFACELOST)
        {
            DDRAW_PRIMARY_SCREEN_SURFACE->Restore();
            ret = DDRAW_PRIMARY_SCREEN_SURFACE->SetPalette(DDRAW_SCREEN_PALETTE);
        }
    }
    return ret;
}


// 0044BCF4 Bedlam 1
void unlock_cursor_surface()
{
    if (SURFACE_IS_LOCKED)
    {
        DDRAW_GAMECURSOR_SURFACE->Unlock(NULL);
    }
}

// 0044BC90 Bedlam 1
uint8_t *lock_and_get_cursor_surface()
{
    uint8_t *result;           // eax
    DDSURFACEDESC description; // [esp+1Ch] [ebp-64h] OVERLAPPED BYREF

    if (!SURFACE_IS_LOCKED)
    {
        return NULL;
    }
    if (DDRAW_GAMECURSOR_SURFACE->IsLost() == DDERR_SURFACELOST)
    {
        DDRAW_GAMECURSOR_SURFACE->Restore();
    }
    description.dwSize = 108;
    if ((uint32_t)DDRAW_GAMECURSOR_SURFACE->Lock(NULL, &description, DDLOCK_WAIT, NULL))
    {
        result = NULL;
    }
    else
    {
        result = (uint8_t *)description.lpSurface;
    }
    return result;
}

// 0044BB84 Bedlam 1
HRESULT Set_Cursor_ColorKey(int16_t color_range)
{
    //DDCOLORKEY colorkey; // [esp+0h] [ebp-14h] BYREF

    //colorkey.dwColorSpaceLowValue = color_range;
    //colorkey.dwColorSpaceHighValue = color_range;
    //return DDRAW_GAMECURSOR_SURFACE->SetColorKey(DDCKEY_SRCBLT, &colorkey);
    return 0 ;
}






// 0044AB54 Bedlam 1
void ddraw_deinit()
{
   /* if (DDRAW_PRIMARY_SCREEN_SURFACE)
    {
        clear_and_blit_screen();
        unlock_surface();
    }
    if (lpDD)
    {
        lpDD->RestoreDisplayMode();
        lpDD->FlipToGDISurface();
        Sleep(500);
    }
    if (DDRAW_SECOND_SCREEN_SURFACE)
    {
        DDRAW_SECOND_SCREEN_SURFACE->Release();
        DDRAW_SECOND_SCREEN_SURFACE = NULL;
    }
    if (DDRAW_PRIMARY_SCREEN_SURFACE)
    {
        DDRAW_PRIMARY_SCREEN_SURFACE->Release();
        DDRAW_PRIMARY_SCREEN_SURFACE = NULL;
    }
    if (DDRAW_SCREEN_PALETTE)
    {
        DDRAW_SCREEN_PALETTE->Release();
        DDRAW_SCREEN_PALETTE = NULL;
    }
    if (DDRAW_GAMECURSOR_SURFACE)
    {
        DDRAW_GAMECURSOR_SURFACE->Release();
        DDRAW_GAMECURSOR_SURFACE = NULL;
    }
    if (DDRAW_MOUSE_BG_SURFACE)
    {
        DDRAW_MOUSE_BG_SURFACE->Release();
        DDRAW_MOUSE_BG_SURFACE = NULL;
    }
    if (lpDD)
    {
        lpDD->Release();
        lpDD = NULL;
    }*/
}
