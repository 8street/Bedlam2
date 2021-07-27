#pragma once
#include <stdint.h>
#include <stdio.h>

void clear_buffer(uint32_t size, uint8_t *buffer);
bool bittest(int32_t value, int32_t bit);
uint8_t BYTE2(uint32_t value);
int get_distance(int delta_x, int delta_y);
long get_file_size(FILE *fp);