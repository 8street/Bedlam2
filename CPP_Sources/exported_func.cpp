#include <cstring>

#include "exported_func.h"
#include "helper.h"

// 00401E39 Bedlam1
void draw_IMG_in_buffer(
    int32_t image_number, int16_t transparent, int32_t y_pos, int32_t x_pos, uint8_t *buffer, const uint8_t *bin_ptr)
{
    int img_header;
    int img_x_pos;
    int img_y_pos;
    int img_x_size;
    int img_height;
    int img_height1;
    int img_x_size1;
    int transparent1;
    const uint8_t *v6;  // esi
    int v7;             // eax
    const uint8_t *v8;  // esi
    int v9;             // eax
    const uint8_t *v10; // esi
    const uint8_t *v12; // esi
    const uint8_t *v13; // esi
    uint8_t *v14;       // edi
    int v16;            // ebx
    bool v17;           // zf
    int v18;            // ecx
    int i;              // ecx
    int v20;            // ecx
    int v21;            // ecx
    int v22;            // ecx
    int v23;            // ecx
    uint8_t *v25;       // [esp-8h] [ebp-8h]
    uint8_t *v26;       // [esp-8h] [ebp-8h]
    char v27;           // [esp-8h] [ebp-8h]
    int v28;            // [esp-8h] [ebp-8h]
    int v29;            // [esp-4h] [ebp-4h]
    int v30;            // [esp-4h] [ebp-4h]
    uint8_t *v31;       // [esp-4h] [ebp-4h]
    uint8_t *v32;       // [esp-4h] [ebp-4h]

    transparent1 = transparent;
    v6 = &bin_ptr[4 * image_number + 2 + *(uint32_t *)&bin_ptr[4 * image_number + 2]];
    v7 = *(uint16_t *)v6;
    v8 = v6 + 2;
    img_header = v7;
    if (bittest(v7, 1u))
    {
        v9 = *(uint16_t *)v8;
        v10 = v8 + 2;
        y_pos += v9;
        x_pos += *(uint16_t *)v10;
        v8 = v10 + 2;
    }
    img_y_pos = y_pos;
    img_x_pos = x_pos;
    img_height1 = *(uint16_t *)v8;
    if (*(uint16_t *)v8)
    {
        img_x_size = *(uint16_t *)v8;
        v12 = v8 + 2;
        img_height1 = *(uint16_t *)v12;
        if (*(uint16_t *)v12)
        {
            v13 = v12 + 2;
            img_height = img_height1;
            v14 = &buffer[640 * img_y_pos + img_x_pos];
            img_x_size1 = img_x_size;
            v16 = img_height1;
            img_height1 = (uint16_t)img_header;
            if (bittest(img_height1, 0))
            {
                if (transparent1)
                {
                    do
                    {
                        v31 = v14;
                        do
                        {
                            while (1)
                            {
                                v18 = *(uint16_t *)v13;
                                v13 += 2;
                                if (!bittest(v18, 0xFu))
                                    break;
                                img_height1 = v18 & 0xFFF;
                                v14 += img_height1;
                                if (bittest(v18, 0xEu))
                                    goto LABEL_28;
                            }
                            img_height1 = v18;
                            v27 = v18;
                            for (i = (uint16_t)(v18 & 0xFFF) >> 1; i; --i)
                            {
                                *(uint16_t *)v14 = *(uint16_t *)v13;
                                v13 += 2;
                                v14 += 2;
                            }
                            if ((v27 & 1) != 0)
                                *v14++ = *v13++;
                        } while (!bittest(img_height1, 0xEu));
                    LABEL_28:
                        v14 = v31 + 640;
                        --v16;
                    } while (v16);
                }
                else
                {
                    do
                    {
                        v32 = v14;
                        do
                        {
                            while (1)
                            {
                                v20 = *(uint16_t *)v13;
                                v13 += 2;
                                if (bittest(v20, 0xFu))
                                    break;
                                img_height1 = v20;
                                v21 = v20 & 0xFFF;
                                memcpy(v14, v13, v21);
                                v13 += v21;
                                v14 += v21;
                                if (bittest(img_height1, 0xEu))
                                    goto LABEL_37;
                            }
                            v28 = v20;
                            v22 = v20 & 0xFFF;
                            img_height1 = 0;
                            do
                            {
                                *v14++ = 0;
                                --v22;
                            } while (v22);
                            v23 = v28;
                        } while (!bittest(v23, 0xEu));
                    LABEL_37:
                        v14 = v32 + 640;
                        --v16;
                    } while (v16);
                }
            }
            else
            {
                v17 = transparent1 == 0;
                if (transparent1)
                {
                    do
                    {
                        v29 = img_x_size1;
                        v25 = v14;
                        do
                        {
                            img_height1 = *v13++;
                            if (img_height1 & 0xFF)
                                *v14 = img_height1;
                            ++v14;
                            --img_x_size1;
                        } while (img_x_size1);
                        v14 = v25 + 640;
                        img_x_size1 = v29;
                        --v16;
                    } while (v16);
                }
                else
                {
                    do
                    {
                        v30 = img_x_size1;
                        v26 = v14;
                        do
                        {
                            memcpy(v14, v13, img_x_size1);
                            v13 += img_x_size1;
                            v14 += img_x_size1;
                            img_x_size1 = 0;
                        } while (!v17);
                        v14 = v26 + 640;
                        img_x_size1 = v30;
                        v17 = --v16 == 0;
                    } while (v16);
                }
            }
        }
    }
}

// 00401471
void draw_tile(int32_t img_num, int32_t ofst, uint8_t *palette, uint8_t *screen_pos, const uint8_t *bin_ptr)
{
    const uint8_t *img_adr;       // esi
    const uint8_t *img_ptr;       // esi
    const uint8_t *v7;            // esi
    int v8;                       // edx
    int v9;                       // edx
    const uint8_t *v11;           // esi
    uint8_t *v12;                 // edi
    const uint8_t *v13;           // esi
    int v14;                      // ebx
    int j;                        // edx
    int v16;                      // ecx
    unsigned int v17;             // ecx
    char v18;                     // cf
    unsigned int v19;             // ecx
    unsigned int v20;             // ecx
    const uint8_t *line_ofst_ptr; // esi
    int line_ofst;                // edx
    int img_ofst;                 // edx
    const uint8_t *v24;           // esi
    uint8_t *screen_pos_1;        // edi
    const uint8_t *v26;           // esi
    int v27;                      // ebx
    int i;                        // edx
    unsigned int v29;             // ecx
    unsigned int v30;             // ecx
    unsigned int v31;             // ecx
    int v32;                      // eax
    const uint8_t *v36;           // esi
    int v37;                      // ecx
    int v38;                      // edx
    int v39;                      // ecx
    int v40;                      // ecx
    int v44;                      // [esp-Ch] [ebp-Ch]
    int v45;                      // [esp-8h] [ebp-8h]
    int v46;                      // [esp-8h] [ebp-8h]
    uint8_t *v47;                 // [esp-4h] [ebp-4h]
    uint8_t *v48;                 // [esp-4h] [ebp-4h]
    uint8_t *palette_ptr = palette;

    const int const1_1 = 1;

    uint16_t unkn1;
    uint32_t unkn2;
    uint16_t unkn3;
    uint32_t unkn4;
    uint32_t unkn6;

    img_adr = &bin_ptr[4 * img_num + 2];
    img_ptr = &img_adr[*(uint32_t *)img_adr];
    if (*(uint16_t *)img_ptr >= 4u)
    {
        line_ofst_ptr = img_ptr + 2;
        line_ofst = 640 * *(uint16_t *)line_ofst_ptr;
        line_ofst_ptr += 2;
        img_ofst = *(uint16_t *)line_ofst_ptr + line_ofst;
        line_ofst_ptr += 2;
        line_ofst = *(uint16_t *)line_ofst_ptr;
        v24 = line_ofst_ptr + 2;
        if (line_ofst)
        {
            screen_pos_1 = &screen_pos[img_ofst];
            unkn1 = *(uint16_t *)v24;
            v26 = v24 + 2;
            v27 = unkn1;
            if (unkn1)
            {
                if (palette_ptr && const1_1)
                {
                LABEL_37:
                    while (2)
                    {
                        v47 = screen_pos_1;
                        while (1)
                        {
                            while (1)
                            {
                                ofst &= 0xFFFFFF00;
                                ofst |= *v26++;
                                v44 = ofst;
                                if (bittest(v44, 7u))
                                    break;
                                ofst = (ofst & 0x3F) + 1;
                                v45 = v27;
                                do
                                {
                                    *screen_pos_1 = palette_ptr[*v26];
                                    ++v26;
                                    ++screen_pos_1;
                                    --ofst;
                                } while (ofst);
                                v27 = v45;
                                if (bittest(v44, 6u))
                                {
                                    screen_pos_1 = v47 + 640;
                                    v27 = v45 - 1;
                                    if (v45 != 1)
                                        goto LABEL_37;
                                    return;
                                }
                            }
                            if (bittest(ofst, 6u))
                                break;
                            screen_pos_1 += (ofst & 0x3F) + 1;
                        }
                        screen_pos_1 = v47 + 640;
                        if (--v27)
                            continue;
                        break;
                    }
                }
                else
                {
                LABEL_24:
                    while (2)
                    {
                        for (i = 640;; i -= v32)
                        {
                            while (1)
                            {
                                ofst &= 0xFFFFFF00;
                                ofst |= *v26++;
                                unkn2 = ofst;
                                if (bittest(unkn2, 7u))
                                    break;
                                v29 = (ofst & 0x3F) + 1;
                                i -= v29;
                                v18 = v29 & 1;
                                v30 = v29 >> 1;
                                if (v18)
                                    *screen_pos_1++ = *v26++;
                                v18 = v30 & 1;
                                v31 = v30 >> 1;
                                if (v18)
                                {
                                    *(uint16_t *)screen_pos_1 = *(uint16_t *)v26;
                                    v26 += 2;
                                    screen_pos_1 += 2;
                                }
                                memcpy(screen_pos_1, v26, 4 * v31);
                                v26 += 4 * v31;
                                screen_pos_1 += 4 * v31;
                                ofst = 0;
                                if (bittest(unkn2, 6u))
                                {
                                    screen_pos_1 += i;
                                    if (--v27)
                                        goto LABEL_24;
                                    return;
                                }
                            }
                            if (bittest(ofst, 6u))
                                break;
                            v32 = (ofst & 0x3F) + 1;
                            screen_pos_1 += v32;
                        }
                        screen_pos_1 += i;
                        if (--v27)
                            continue;
                        break;
                    }
                }
            }
        }
    }
    else if (*(uint16_t *)img_ptr)
    {
        v7 = img_ptr + 2;
        v8 = 640 * *(uint16_t *)v7;
        v7 += 2;
        v9 = *(uint16_t *)v7 + v8;
        v7 += 2;
        unkn3 = *(uint16_t *)v7;
        v11 = v7 + 2;
        if (unkn3)
        {
            v12 = &screen_pos[v9];
            unkn3 = *(uint16_t *)v11;
            v13 = v11 + 2;
            v14 = unkn3;
            if (unkn3)
            {
                if (palette_ptr && const1_1)
                {
                LABEL_55:
                    while (2)
                    {
                        v48 = v12;
                        while (1)
                        {
                            while (1)
                            {
                                v39 = *(uint16_t *)v13;
                                v13 += 2;
                                unkn4 = v39;
                                if (bittest(unkn4, 0xFu))
                                    break;
                                v40 = v39 & 0xFFF;
                                v46 = v14;
                                do
                                {
                                    *v12 = palette_ptr[*v13];
                                    ++v13;
                                    ++v12;
                                    --v40;
                                } while (v40);
                                v14 = v46;
                                if (bittest(unkn4, 0xEu))
                                {
                                    v12 = v48 + 640;
                                    v14 = v46 - 1;
                                    if (v46 != 1)
                                        goto LABEL_55;
                                    return;
                                }
                            }
                            if (bittest(v39, 0xEu))
                                break;
                            v12 += v39 & 0xFFF;
                        }
                        v12 = v48 + 640;
                        if (--v14)
                            continue;
                        break;
                    }
                }
                else
                {
                LABEL_7:
                    while (2)
                    {
                        for (j = 640;; j -= v16 & 0xFFF)
                        {
                            while (1)
                            {
                                v16 = *(uint16_t *)v13;
                                v13 += 2;
                                unkn6 = v16;
                                if (bittest(unkn6, 0xFu))
                                    break;
                                v17 = v16 & 0xFFF;
                                j -= v17;
                                v18 = v17 & 1;
                                v19 = v17 >> 1;
                                if (v18)
                                    *v12++ = *v13++;
                                v18 = v19 & 1;
                                v20 = v19 >> 1;
                                if (v18)
                                {
                                    *(uint16_t *)v12 = *(uint16_t *)v13;
                                    v13 += 2;
                                    v12 += 2;
                                }
                                memcpy(v12, v13, 4 * v20);
                                v13 += 4 * v20;
                                v12 += 4 * v20;
                                if (bittest(unkn6, 0xEu))
                                {
                                    v12 += j;
                                    if (--v14)
                                        goto LABEL_7;
                                    return;
                                }
                            }
                            if (bittest(v16, 0xEu))
                                break;
                            v12 += v16 & 0xFFF;
                        }
                        v12 += j;
                        if (--v14)
                            continue;
                        break;
                    }
                }
            }
        }
    }
    else
    {
        v36 = img_ptr + 6;
        v37 = 64;
        do
        {
            v38 = 64;
            do
            {
                if (*v36)
                    *screen_pos = *v36;
                ++v36;
                ++screen_pos;
                --v38;
            } while (v38);
            screen_pos += 576;
            --v37;
        } while (v37);
    }
}
