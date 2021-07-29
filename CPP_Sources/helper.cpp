#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "helper.h"

// 00402965 Bedlam 1
void clear_buffer(uint32_t size, uint8_t *buffer)
{
    memset(buffer, 0, size);
}

// not present
bool bittest(int32_t value, int32_t bit)
{
    return value & (1 << bit);
}

// not present
uint8_t BYTE2(uint32_t value)
{
    uint32_t ret = (value & 0x00FF0000) >> 16;
    return static_cast<uint8_t>(ret);
}

// 0041EBF8 Bedlam 1
// Hack equival SQRT(X^2 + Y^2)
int get_distance(int delta_x, int delta_y)
{
    int x1 = abs(delta_x);
    int y1 = abs(delta_y);
    if (x1 <= y1)
    {
        x1 >>= 1;
    }
    else
    {
        y1 >>= 1;
    }
    return x1 + y1;
}

// 000305D4 Bedlam 2
long get_file_size(FILE *fp)
{
    long curr_pos = ftell(fp);
    fseek(fp, 0L, SEEK_END);
    long sz = ftell(fp);
    fseek(fp, curr_pos, SEEK_SET);
    return sz;
}
