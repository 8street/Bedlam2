#include <cstdint>
#include <iostream>
#include <stdio.h>
#include <string>

#include "bedlam2_draw.h"
#include "helper.h"
#include "palette.h"
#include "screenshot.h"
#include "sdl_window.h"

#ifdef _MSC_VER
#    pragma warning(disable : 4996)
#endif

int NUM_SCREENSHOTS;

// 0044CEB0 Bedlam 1
int save_screenshot()
{
    int n;                            // esi
    int pal_indx;                     // eax
    FILE *file;                       // eax
    int ii;                           // esi
    int bytes_writted;                // ebp
    int i;                            // esi
    uint8_t *current_screen_point;    // eax
    uint8_t buffer1_1[258 * 3];       // [esp+0h] [ebp-38Ch]
    BITMAPINFOHEADER bmp_info_header; // [esp+304h] [ebp-88h] BYREF
    char filename[32];                // [esp+32Ch] [ebp-60h] BYREF
    BITMAPFILEHEADER bmp_file_header; // [esp+34Ch] [ebp-40h] BYREF
    uint32_t filesize;                // [esp+364h] [ebp-28h] BYREF
    uint32_t null;                    // [esp+368h] [ebp-24h] BYREF
    uint8_t *screen_surface_ptr;      // [esp+36Ch] [ebp-20h]
    int height;                       // [esp+370h] [ebp-1Ch]

    n = 0;
    pal_indx = 0;
    null = 0;
    uint8_t *pal = GAME_WINDOW.get_RGB_palette_ptr();
    memset(buffer1_1, 0, sizeof(buffer1_1));
    do
    {
        n += 3;
        buffer1_1[n + 1] = pal[pal_indx * 3] * 4;
        buffer1_1[n + 2] = pal[pal_indx * 3 + 1] * 4;
        buffer1_1[n + 3] = pal[pal_indx * 3 + 2] * 4;
        pal_indx++;
    } while (pal_indx != 256);
    while (1)
    {
        sprintf(filename, "SS%04d.BMP", NUM_SCREENSHOTS);
        NUM_SCREENSHOTS++;
        if (NUM_SCREENSHOTS > 9999)
        {
            NUM_SCREENSHOTS = 9999;
        }
        file = fopen(filename, "rb");
        if (!file)
        {
            break;
        }
        fclose(file);
    }
    file = fopen(filename, "wb");
    if (file)
    {
        bmp_file_header.bfType = 'MB';
        bmp_file_header.bfSize = 0;
        bmp_file_header.bfReserved1 = 0;
        bmp_file_header.bfReserved2 = 0;
        bmp_file_header.bfOffBits = 0;
        fwrite((char *)&bmp_file_header, 1, sizeof(bmp_file_header), file);
        bmp_info_header.biCompression = 0;
        bmp_info_header.biYPelsPerMeter = 0;
        bmp_info_header.biClrImportant = 0;
        bmp_info_header.biSize = sizeof(bmp_info_header);
        bmp_info_header.biBitCount = 8;
        bmp_info_header.biSizeImage = 0;
        bmp_info_header.biWidth = GAME_WINDOW.get_game_width();
        bmp_info_header.biXPelsPerMeter = 0;
        bmp_info_header.biClrUsed = 0;
        bmp_info_header.biHeight = GAME_WINDOW.get_game_height();
        bmp_info_header.biPlanes = 1;
        fwrite((char *)&bmp_info_header, 1, sizeof(bmp_info_header), file);
        ii = 0;
        do
        {
            fputc((uint8_t)buffer1_1[ii + 6], file);
            fputc((uint8_t)buffer1_1[ii + 5], file);
            fputc((uint8_t)buffer1_1[ii + 4], file);
            ii += 3;
            fputc(0, file);
        } while (ii != 768);
        filesize = (uint32_t)get_file_size(file);
        fseek(file, 10, SEEK_SET);
        fwrite(&filesize, 1, sizeof(filesize), file);
        fseek(file, 0, SEEK_END);
        screen_surface_ptr = GAME_WINDOW.lock_and_get_surface_ptr();
        height = GAME_WINDOW.get_game_height() - 1;
        if (height > -1)
        {
            bytes_writted = 0;
            for (i = 0;; i = 0)
            {
                while (i < GAME_WINDOW.get_game_width())
                {
                    current_screen_point = &screen_surface_ptr[i + SCREEN_SURFACE_WIDTH * height];
                    bytes_writted += fwrite(current_screen_point, 1, 1, file);
                    i++;
                }
                if ((bytes_writted & 3) != 0)
                    fwrite((char *)&null, 1, 4 - (bytes_writted & 3), file);
                if (--height <= -1)
                    break;
                bytes_writted = 0;
            }
        }
        filesize = (uint32_t)get_file_size(file);
        fseek(file, 2, SEEK_SET);
        fwrite(&filesize, 1, sizeof(filesize), file);
        fseek(file, 0, SEEK_END);
        fclose(file);
        GAME_WINDOW.unlock_screen_surface();
        return 0;
    }
    else
    {
        std::cout << "Unable to save screenshot. The file " << filename << " does not open." << std::endl;
        return -1;
    }
}