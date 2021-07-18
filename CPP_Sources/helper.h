#pragma once
#include <stdint.h>
#include <stdio.h>

void _fastcall clear_buffer(uint32_t size, uint8_t* buffer);
bool _fastcall bittest(int32_t value, int32_t bit);
uint8_t _fastcall BYTE2(uint32_t value);
int _fastcall get_distance(int delta_x, int delta_y);
long get_file_size(FILE* fp);