#include "bedlam2_draw.h"
#include "tiles.h"

Tiles::Tiles()
{
}

Tiles::~Tiles()
{
    delete[] Z_BUFFER_PTR;
    Z_BUFFER_PTR = nullptr;
}

int Tiles::init_vars()
{
    // After changing game width and height, you should change these vars
    VETRICAL_TILE_LEVEL_OFFSET = 2;
    NUM_TILES_X = 36;
    NUM_TILES_Y = 36;
    NOT_VISIBLE_TILES_X = 9;
    NOT_VISIBLE_TILES_Y = 9;

    if (Z_BUFFER_PTR)
    {
        delete[] Z_BUFFER_PTR;
        Z_BUFFER_PTR = nullptr;
    }
    // 8 is max z levels count
    Z_BUFFER_PTR = new uint32_t[NUM_TILES_X * NUM_TILES_Y * 8]();
    return 0;
}