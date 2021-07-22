#pragma once
#include <stdint.h>

extern volatile uint32_t PALETTE_TIMER;

extern "C" void set_palette(uint8_t *pal_file);
void palette_animation();
extern "C" void swap_palette_with_animation(uint8_t *palette_file_ptr, int time);
uint8_t *get_RGB_palette_ptr();
extern "C" void wait_palette_animation(int target_time);
