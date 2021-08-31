#pragma once
#include <cstdint>
////////////////////////////////////////////////////
// Here are not parsed functions from bedlam2.asm //
////////////////////////////////////////////////////

void draw_IMG_in_buffer(
    int32_t image_number, int16_t transparent, int32_t y_pos, int32_t x_pos, uint8_t *buffer, const uint8_t *bin_ptr);
void draw_tile(int32_t img_num, int32_t ofst, uint8_t *palette, uint8_t *screen_pos, const uint8_t *bin_ptr);