#include <stdio.h>

#include "bedlam2.h"
#include "ddraw_func.h"
#include "main.h"
#include "mouse.h"
#include "timers.h"
#include "window.h"

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

uint8_t* SCREEN_BUFFER_PTR;

uint8_t GAME_SCREEN_PTR[409600];

//0044A5F0 Bedlam 1
int ddraw_init(HWND hWnd) {
    HRESULT retcode;
    int retVal;
    DWORD flags;


    DDRAW_CREATED = 1;
    retcode = DirectDrawCreate(NULL, &lpDD, NULL);
    if ((uint32_t)retcode)
    {
        if (retcode == DDERR_NODIRECTDRAWSUPPORT) {
            retVal = 1010;
        }
        else {
            retVal = 1001;
        }
    }
    else
    {
        if (FULLSCREEN) {
            flags = DDSCL_EXCLUSIVE | DDSCL_NOWINDOWCHANGES | DDSCL_FULLSCREEN;
        }
        else {
            flags = DDSCL_NORMAL;
        }
        if ((uint32_t)lpDD->SetCooperativeLevel(hWnd, flags)) {
            retVal = 1002;
        }
        else {
            retVal = 0;
        }
    }
    return retVal;
}

//0044A660 Bedlam 1
int create_surface_palette(int32_t width, int32_t height, int32_t depth) {

    volatile HRESULT ret_code; // eax
    void* v6; // ebx
    int v7; // ecx



    DDSURFACEDESC ddraw_surf_descr2; // [esp+2Ch] [ebp-80h] BYREF
    DDSCAPS caps; // [esp+98h] [ebp-14h] OVERLAPPED BYREF

    if (FULLSCREEN)
    {
        ret_code = lpDD->SetDisplayMode(width, height, depth);
        if ((int32_t)ret_code)
            return 3;
    }
    else
    {
        ddraw_surf_descr2.dwSize = sizeof(ddraw_surf_descr2);
        lpDD->GetDisplayMode(&ddraw_surf_descr2);
        if ((ddraw_surf_descr2.ddpfPixelFormat.dwFlags & DDSD_BACKBUFFERCOUNT) == 0)
            return 1003;
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

    if (FULLSCREEN)
    {
        memset(&ddraw_surf_descr2, 0, sizeof(ddraw_surf_descr2));
        ddraw_surf_descr2.dwSize = sizeof(ddraw_surf_descr2);
        ddraw_surf_descr2.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
        ddraw_surf_descr2.dwBackBufferCount = 1;
        ddraw_surf_descr2.ddsCaps.dwCaps = DDSCAPS_COMPLEX | DDSCAPS_FLIP | DDSCAPS_PRIMARYSURFACE | DDSCAPS_VIDEOMEMORY;
        if (USE_VIDEOMEMORY)
            ret_code = lpDD->CreateSurface(&ddraw_surf_descr2, &DDRAW_PRIMARY_SCREEN_SURFACE, NULL);
        else
            ret_code = 1;
        //word_4EE9E4 = 0;
        if ((uint32_t)ret_code)
        {
            ddraw_surf_descr2.ddsCaps.dwCaps = DDSCAPS_COMPLEX | DDSCAPS_FLIP | DDSCAPS_PRIMARYSURFACE;
            ret_code = lpDD->CreateSurface(&ddraw_surf_descr2, &DDRAW_PRIMARY_SCREEN_SURFACE, NULL);
        }
        if ((uint32_t)ret_code)
            return 1004;
        v6 = &caps;
        v7 = 4;
        caps.dwCaps = DDSCAPS_BACKBUFFER;
        if ((uint32_t)DDRAW_PRIMARY_SCREEN_SURFACE->GetAttachedSurface(&caps, &DDRAW_SECOND_SCREEN_SURFACE))
            return 1004;
    }
    else {
        memset(&ddraw_surf_descr2, 0, sizeof(ddraw_surf_descr2));
        ddraw_surf_descr2.dwSize = sizeof(ddraw_surf_descr2);
        ddraw_surf_descr2.ddsCaps.dwCaps = DDCAPS_BLTSTRETCH | DDSCAPS_VIDEOMEMORY;
        ddraw_surf_descr2.dwFlags = DDSD_CAPS;
        if ((uint32_t)lpDD->CreateSurface(&ddraw_surf_descr2, &DDRAW_PRIMARY_SCREEN_SURFACE, NULL))
            return 1004;
        v7 = 64;
        memset(&ddraw_surf_descr2, 0, sizeof(ddraw_surf_descr2));
        ddraw_surf_descr2.dwWidth = width;
        ddraw_surf_descr2.dwHeight = height;
        ddraw_surf_descr2.ddsCaps.dwCaps = DDCAPS_BLT | DDSCAPS_VIDEOMEMORY;
        ddraw_surf_descr2.dwSize = sizeof(ddraw_surf_descr2);
        ddraw_surf_descr2.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
        if ((uint32_t)lpDD->CreateSurface(&ddraw_surf_descr2, &DDRAW_SECOND_SCREEN_SURFACE, NULL))
            return 1004;
        //v8 = lpDD->lpVtbl;
        if ((uint32_t)lpDD->CreateClipper(NULL, &DDRAW_SURFACE_CLIPPER, NULL))
            return 1004;
        DDRAW_SURFACE_CLIPPER->SetHWnd(NULL, WINDOW_HWND);
        DDRAW_PRIMARY_SCREEN_SURFACE->SetClipper(DDRAW_SURFACE_CLIPPER);
    }
    HDC dc = GetDC(0);
    uint32_t j = 0;
    uint32_t i = 0;
    if ((GetDeviceCaps(dc, RASTERCAPS) & RC_PALETTE) != 0)
    {
        if (FULLSCREEN)
        {
            do
            {
                j++;
                PALETTEENTRY_BUFFER[j].peRed = 0;
                PALETTEENTRY_BUFFER[j].peGreen = 0;
                PALETTEENTRY_BUFFER[j].peBlue = 0;
                PALETTEENTRY_BUFFER[j].peFlags = PC_RESERVED;
            } while (j != 256);
        }
        else {
            do
            {
                j++;
                PALETTEENTRY_BUFFER[j].peRed = 0;
                PALETTEENTRY_BUFFER[j].peGreen = 0;
                PALETTEENTRY_BUFFER[j].peBlue = 0;
                PALETTEENTRY_BUFFER[j].peFlags = PC_RESERVED;
            } while (j != 256);

            PALETTEENTRY_BUFFER[256].peRed = -1;
            PALETTEENTRY_BUFFER[256].peGreen = -1;
            PALETTEENTRY_BUFFER[256].peBlue = -1;
        }
        if (lpDD->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256, &PALETTEENTRY_BUFFER[1], &DDRAW_SCREEN_PALETTE, NULL) == DD_OK) {
            DDRAW_PRIMARY_SCREEN_SURFACE->SetPalette(DDRAW_SCREEN_PALETTE);
        }
    }
    ReleaseDC(0, dc);
    create_mouse_buffer();
    return 0;
}

//0044BA3C Bedlam 1
HRESULT create_mouse_buffer()
{
    HRESULT retVal; // eax
    //HRESULT v1; // eax
    HRESULT result; // eax
    //char* surface; // esi
    //int j; // edx
    //int v5; // eax
    //char* i; // ebx
    DDSURFACEDESC surfDescr5; // [esp+24h] [ebp-8Ch] BYREF
    DDCOLORKEY color_key; // [esp+90h] [ebp-20h] BYREF

    memset(&surfDescr5, 0, sizeof(surfDescr5));
    surfDescr5.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
    surfDescr5.dwWidth = 32;
    surfDescr5.dwHeight = 32;
    surfDescr5.dwSize = sizeof(surfDescr5);
    surfDescr5.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    retVal = lpDD->CreateSurface(&surfDescr5, &DDRAW_GAMECURSOR_SURFACE, NULL);
    if ((uint32_t)retVal)
        printf("Error creating  mouse buffer: %x\n", retVal);
    retVal = lpDD->CreateSurface(&surfDescr5, &DDRAW_MOUSE_BG_SURFACE, NULL);
    if ((uint32_t)retVal)
        printf("Error creating mouse bg buffer: %x\n", retVal);
    surfDescr5.dwSize = sizeof(surfDescr5);
    result = DDRAW_GAMECURSOR_SURFACE->Lock(NULL, &surfDescr5, DDLOCK_WAIT, NULL);
    if (!(uint32_t)result)
    {
        DDRAW_GAMECURSOR_SURFACE->Unlock(NULL);
        color_key.dwColorSpaceLowValue = 0;
        color_key.dwColorSpaceHighValue = 0;
        result = DDRAW_GAMECURSOR_SURFACE->SetColorKey(DDCKEY_SRCBLT, &color_key);
        CURSOR_SURFACE_SIZE = 32;
    }
    return result;
}

//0044A9AC Bedlam 1
void clear_and_blit_screen() 
{
    int app_active1; // edi
    uint8_t* surface_ptr; // eax
    int attempt; // edx
    uint8_t* surface_ptr3; // eax
    int attempt1; // edx
    int attempt2; // edx
    uint8_t* surface_ptr1; // ebx
    int32_t y; // esi
    int32_t x; // ecx
    uint8_t* surface_ptr2; // ebx
    int attempt3; // ecx

    if (SURFACE_IS_LOCKED)
    {
        app_active1 = APPLICATION_ACTIVE;
        APPLICATION_ACTIVE = 1;
        if (SURFACE_IS_LOCKED)
        {
            unlock_surface();
            blit_second_surface_to_screen();
            surface_ptr = 0;
            attempt = 0;
            do
            {
                surface_ptr = lock_and_get_surface_ptr();
                attempt++;
            } while (!surface_ptr && attempt < 20);
            if (!surface_ptr)
            {
                APPLICATION_ACTIVE = app_active1;
                return;
            }
            *(DWORD*)surface_ptr = 0x12345678;
            unlock_surface();
            blit_second_surface_to_screen();
            attempt1 = 0;
            surface_ptr3 = 0;
            do
            {
                if (attempt1 >= 20)
                    break;
                surface_ptr3 = lock_and_get_surface_ptr();
                ++attempt1;
            } while (!surface_ptr3);
            if (surface_ptr3)
            {
                //if (*(DWORD*)surface_ptr3 == 0x12345678)
                //    word_4EE9E4 = 1;
                unlock_surface();
                blit_second_surface_to_screen();
                attempt2 = 0;
                while (attempt2 < 20)
                {
                    surface_ptr1 = lock_and_get_surface_ptr();
                    ++attempt2;
                    if (surface_ptr1)
                    {
                        for (y = 0; y < GAME_HEIGHT; ++y)
                        {
                            for (x = 0; x < GAME_WIDTH; ++x)
                            {
                                surface_ptr1 = 0;
                                surface_ptr1++;
                            }
                            surface_ptr1 += SCREEN_SURFACE_WIDTH - GAME_WIDTH;
                        }
                        unlock_surface();
                        blit_second_surface_to_screen();
                        surface_ptr2 = 0;
                        attempt3 = 0;
                        do
                        {
                            if (attempt3 >= 20)
                                break;
                            surface_ptr2 = lock_and_get_surface_ptr();
                            ++attempt3;
                        } while (!surface_ptr2);
                        if (surface_ptr2)
                        {
                            for (y = 0; y < GAME_HEIGHT; ++y)
                            {
                                for (x = 0; x < GAME_WIDTH; ++x)
                                {
                                    *surface_ptr2 = 0;
                                    surface_ptr2++;
                                }
                                surface_ptr2 += SCREEN_SURFACE_WIDTH - GAME_WIDTH;
                            }
                            unlock_surface();
                            blit_second_surface_to_screen();
                        }
                        break;
                    }
                }
            }
        }
        APPLICATION_ACTIVE = app_active1;
    }
}

//0044ACF4 Bedlam 1
HRESULT unlock_surface()
{
    HRESULT result; // eax

    result = SURFACE_IS_LOCKED;
    if (SURFACE_IS_LOCKED) {
        result = DDRAW_SECOND_SCREEN_SURFACE->Unlock(NULL);
    }
    return result;
}

//0044AC5C Bedlam 1
uint8_t* lock_and_get_surface_ptr() {

    DDSURFACEDESC surface_description; // [esp+20h] [ebp-64h] OVERLAPPED BYREF

    if (!APPLICATION_ACTIVE || !SURFACE_IS_LOCKED) {
        return NULL;
    }
    if (DDRAW_PRIMARY_SCREEN_SURFACE->IsLost() == DDERR_SURFACELOST) {
        DDRAW_PRIMARY_SCREEN_SURFACE->Restore();
    }
    surface_description.dwSize = sizeof(surface_description);
    if (DDRAW_SECOND_SCREEN_SURFACE->Lock(NULL, &surface_description, DDLOCK_WAIT, NULL) != DD_OK) {
        return NULL;
    }
    SCREEN_SURFACE_WIDTH = surface_description.lPitch;
    SCREEN_SURFACE_HEIGHT = surface_description.dwHeight;

    return (uint8_t*)(surface_description.lpSurface);
}

//0044B7B0 Bedlam 1
int blit_cursor_bg_to_screen()
{
    int result = 0; // eax
    uint16_t drawing_cursor_pos_x; // bx
    uint16_t drawing_cursor_pos_y; // dx
    tagRECT destination; // [esp+28h] [ebp-1Ch] BYREF
    tagRECT source; // [esp+38h] [ebp-Ch] OVERLAPPED BYREF

    if (DDRAW_PRIMARY_SCREEN_SURFACE)
    {
        result = CURSOR_X1;
        if (CURSOR_X1 != -1)
        {
            if (DDRAW_PRIMARY_SCREEN_SURFACE->IsLost() == DDERR_SURFACELOST)
            {
                DDRAW_PRIMARY_SCREEN_SURFACE->Restore();
            }
            if (DDRAW_MOUSE_BG_SURFACE->IsLost() == DDERR_SURFACELOST) {
                DDRAW_MOUSE_BG_SURFACE->Restore();
            }
            drawing_cursor_pos_x = WINDOW_POS_X + GAME_WIDTH - CURSOR_X1;
            if (drawing_cursor_pos_x > CURSOR_SURFACE_SIZE) {
                drawing_cursor_pos_x = CURSOR_SURFACE_SIZE;
            }
            drawing_cursor_pos_y = WINDOW_POS_Y + GAME_HEIGHT - CURSOR_Y1;
            if (drawing_cursor_pos_y > CURSOR_SURFACE_SIZE) {
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

//0044B6D4 Bedlam 1
void blit_cursor_bg()
{
    if (1)
    {
        blit_cursor_bg_to_screen();
    }
    CURSOR_X1 = -1;
}

// 0044AD18
int blit_second_surface_to_screen()
{
    int result = 0; // eax
    tagRECT destination; // [esp+0h] [ebp-3Ch] BYREF
    tagRECT source; // [esp+10h] [ebp-2Ch] BYREF
    struct tagPOINT Cursor; // [esp+20h] [ebp-1Ch] BYREF

    if (APPLICATION_ACTIVE)
    {
        result = SURFACE_IS_LOCKED;
        IS_BLITTING = 1;
        if (SURFACE_IS_LOCKED)
        {
            if (UPDATE_CURSOR_BY_TIMER)
            {
                while (CURSOR_IS_BLITTING) {
                    ;
                }
                CURSOR_UNKNOWN = 1;
                blit_cursor_bg();
            }
            if (FULLSCREEN)
            {
                DDRAW_PRIMARY_SCREEN_SURFACE->Flip(NULL, DDFLIP_WAIT);
            }
            else
            {
                source.left = 0;
                source.top = 0;
                source.right = GAME_WIDTH;
                source.bottom = GAME_HEIGHT;
                destination.left = WINDOW_POS_X;
                destination.top = WINDOW_POS_Y;
                destination.right = WINDOW_POS_X + GAME_WIDTH;
                destination.bottom = WINDOW_POS_Y + GAME_HEIGHT;
                DDRAW_PRIMARY_SCREEN_SURFACE->Blt(&destination, DDRAW_SECOND_SCREEN_SURFACE, &source, 0x1000000, NULL);
            }
            if (UPDATE_CURSOR_BY_TIMER)
            {
                CURSOR_UNKNOWN = 0;
                GetCursorPos(&Cursor);
                blit_cursor(Cursor.x, Cursor.y);
            }
            result = NEED_PALETTE_UPDATE;
            if (NEED_PALETTE_UPDATE) {
                result = DDRAW_PRIMARY_SCREEN_SURFACE->SetPalette(DDRAW_SCREEN_PALETTE);
            }
            NEED_PALETTE_UPDATE = 0;
        }
        IS_BLITTING = 0;
    }
    return result;
}

//0044B4FC Bedlam 1
int blit_cursor(int x, int y)
{
    int result = 0; // eax
    __int16 size_x; // ax
    __int16 size_y; // dx
    tagRECT cursor_rect; // [esp+30h] [ebp-34h] BYREF
    tagRECT screen_rect; // [esp+40h] [ebp-24h] BYREF

    if (x < WINDOW_POS_X) {
        x = WINDOW_POS_X;
    }
    if (x >= WINDOW_POS_X + GAME_WIDTH) {
        x = WINDOW_POS_X + GAME_WIDTH;
    }
    if (y < WINDOW_POS_Y) {
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
                if (DDRAW_PRIMARY_SCREEN_SURFACE->IsLost() == DDERR_SURFACELOST) {
                    DDRAW_PRIMARY_SCREEN_SURFACE->Restore();
                }
                if (DDRAW_GAMECURSOR_SURFACE->IsLost() == DDERR_SURFACELOST) {
                    DDRAW_GAMECURSOR_SURFACE->Restore();
                }
                if (DDRAW_MOUSE_BG_SURFACE->IsLost() == DDERR_SURFACELOST)
                    DDRAW_MOUSE_BG_SURFACE->Restore();
                blit_cursor_bg_to_screen();
                size_x = WINDOW_POS_X + GAME_WIDTH - x;
                if (size_x > CURSOR_SURFACE_SIZE) {
                    size_x = CURSOR_SURFACE_SIZE;
                }
                size_y = WINDOW_POS_Y + GAME_HEIGHT - y;
                if (size_y > CURSOR_SURFACE_SIZE) {
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

//0044B1C0 Bedlam 1
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

//00425A8B Bedlam 1
void get_screen_buffer_ptr()
{
    while (!SCREEN_BUFFER_PTR) {
        SCREEN_BUFFER_PTR = lock_and_get_surface_ptr();
    }
    if (DDRAW_PRIMARY_SCREEN_SURFACE->IsLost() == DDERR_SURFACELOST) {
        DDRAW_PRIMARY_SCREEN_SURFACE->Restore();
    }
    if (DDRAW_SECOND_SCREEN_SURFACE->IsLost() == DDERR_SURFACELOST) {
        DDRAW_SECOND_SCREEN_SURFACE->Restore();
    }
}

//00425AA0 Bedlam 1
HRESULT unlock_surface_and_screen_ptr()
{
    HRESULT result = DD_OK; // eax

    if (SCREEN_BUFFER_PTR)
    {
        result = unlock_surface();
        SCREEN_BUFFER_PTR = 0;
    }
    return result;
}

//0044AED4 Bedlam 1
HRESULT ddraw_setpalettes(uint8_t* pal_ptr, int16_t offset, int16_t num_entries)
{
    int32_t entry; // edx
    int32_t ofst; // ebx
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
        //if (first_entry < 0)
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
        if (!FULLSCREEN) {
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

//00425A03 Bedlam 1
void redraw()
{
    unlock_surface();
    SCREEN_BUFFER_PTR = 0;
    blit_second_surface_to_screen();
}

//00425A1E Bedlam 1
void copy_buffer_to_screen_and_unlock(uint8_t* buffer)
{
    int i; // [esp+0h] [ebp-20h]
    uint8_t* destin; // [esp+4h] [ebp-1Ch]

    get_screen_buffer_ptr();
    destin = SCREEN_BUFFER_PTR;
    for (i = 0; i < (int32_t)GAME_HEIGHT; ++i)
    {
        memcpy(destin, buffer, SCREEN_SURFACE_WIDTH);
        buffer += SCREEN_SURFACE_WIDTH;
        destin += SCREEN_SURFACE_WIDTH;
    }
    unlock_surface_and_screen_ptr();
}

//0044BCF4 Bedlam 1
void unlock_cursor_surface()
{
    if (SURFACE_IS_LOCKED) {
        DDRAW_GAMECURSOR_SURFACE->Unlock(NULL);
    }
}

//0044BC90 Bedlam 1
uint8_t* lock_and_get_cursor_surface()
{
    uint8_t* result; // eax
    DDSURFACEDESC description; // [esp+1Ch] [ebp-64h] OVERLAPPED BYREF

    if (!SURFACE_IS_LOCKED) {
        return NULL;
    }
    if (DDRAW_GAMECURSOR_SURFACE->IsLost() == DDERR_SURFACELOST) {
        DDRAW_GAMECURSOR_SURFACE->Restore();
    }
    description.dwSize = 108;
    if ((uint32_t)DDRAW_GAMECURSOR_SURFACE->Lock(NULL, &description, DDLOCK_WAIT, NULL))
    {
        result = NULL;
    }
    else
    {
        result = (uint8_t*)description.lpSurface;
    }
    return result;
}

//0044BB84 Bedlam 1
HRESULT Set_Cursor_ColorKey(int16_t color_range)
{
    DDCOLORKEY colorkey; // [esp+0h] [ebp-14h] BYREF

    colorkey.dwColorSpaceLowValue = color_range;
    colorkey.dwColorSpaceHighValue = color_range;
    return DDRAW_GAMECURSOR_SURFACE->SetColorKey(DDCKEY_SRCBLT, &colorkey);
}

//0042597C Bedlam 1
void clear_screen()
{
    get_screen_buffer_ptr();
    memset(SCREEN_BUFFER_PTR, 0, SCREEN_SURFACE_WIDTH * GAME_HEIGHT);
    redraw();

    get_screen_buffer_ptr();
    memset(SCREEN_BUFFER_PTR, 0, SCREEN_SURFACE_WIDTH * GAME_HEIGHT);
    redraw();
}

//0041E215 Bedlam 1
int unlock_surface_and_wait(int time_to_waiting)
{
    int result = unlock_surface_and_screen_ptr();
    WAITING_TIMER = 0;
    while (time_to_waiting > WAITING_TIMER) {

    }
    return result;
}

//0001256B Bedlam 1
void redraw_copy_buff_to_screen_and_unlock(uint8_t* screen_buf)
{
    redraw();
    copy_buffer_to_screen_and_unlock(screen_buf);
}

//0040129E Bedlam 1
int clear_game_viewport()
{
    uint8_t* destination; // edi
    int line; // edx

    get_screen_buffer_ptr();
    destination = SCREEN_BUFFER_PTR;
    line = SCREEN_SURFACE_HEIGHT;
    do
    {
        memset(destination, 0, GAME_WIDTH - SIDEBAR_WIDTH);
        destination += SCREEN_SURFACE_WIDTH;
        --line;
    } while (line);
    return 0;
}

//00401107 Bedlam 1
// Copy map or game screen in SCREEN_BUFFER_PTR
void draw_map_and_game_screen()
{
    uint8_t* source_ptr;
    uint8_t* destination_ptr;
    if (dead_screen_scale)
    {
        dead_screen_scaler();
    }
    else
    {
        if (map_active) {
            source_ptr = game_screen_ptr;
        }
        else
        {
            source_ptr = &game_screen_ptr[GAME_WIDTH * (((screen_y_pos & 31) + (screen_x_pos & 31u)) >> 1) // y
                + 64 * GAME_WIDTH                                                                // y offset (64 is tile size)
                + 64                                                                             // x offset
                + (((screen_x_pos & 31) - (screen_y_pos & 31) + 32) & 63)];                      // x
        }
        get_screen_buffer_ptr();
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

//0044AB54 Bedlam 1
void ddraw_deinit()
{
    if (DDRAW_PRIMARY_SCREEN_SURFACE)
    {
        clear_and_blit_screen();
        unlock_surface();
    }
    if (lpDD) {
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
    }
}

//004012F7 Bedlam 1
void dead_screen_scaler()
{
    clear_game_viewport();
    uint8_t* source_ptr = &game_screen_ptr[GAME_WIDTH * (((screen_y_pos & 31) + (screen_x_pos & 31u)) >> 1)   // y
        + 64 * GAME_WIDTH                                                                                     // y offset (64 is tile size)
        + 64                                                                                                  // x offset
        + (((screen_x_pos & 31) - (screen_y_pos & 31) + 32) & 63)];                                           // x
    int scaled_screen_width = GAME_WIDTH - dead_screen_scale - SIDEBAR_WIDTH;
    int scaled_screen_height = GAME_HEIGHT - dead_screen_scale;
    uint8_t* destination_ptr = SCREEN_BUFFER_PTR; 

    for (int y = 0; y < scaled_screen_height; y++) {
        for (int x = 0; x < scaled_screen_width; x++)
        {
            destination_ptr[x                                                               // x
                            + y * GAME_WIDTH                                                // y
                            + dead_screen_scale / 2                                         // offset x
                            + dead_screen_scale / 2 * GAME_WIDTH]                           // offset y
                            = source_ptr[
                            x * (GAME_WIDTH - SIDEBAR_WIDTH) / scaled_screen_height +       // x
                            y * GAME_HEIGHT / scaled_screen_height * SCREEN_SURFACE_WIDTH]; // y
        }
    }
}

