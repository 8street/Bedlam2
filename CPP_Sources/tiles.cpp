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

int Tiles::init_vars(const Resolution_settings &settings)
{
    VETRICAL_TILE_LEVEL_OFFSET = settings.m_vertical_tile_offset;
    NUM_TILES_X = settings.m_num_tiles_x;
    NUM_TILES_Y = settings.m_num_tiles_y;
    NOT_VISIBLE_TILES_X = settings.m_not_visible_tiles_x;
    NOT_VISIBLE_TILES_Y = settings.m_not_visible_tiles_y;

    if (Z_BUFFER_PTR)
    {
        delete[] Z_BUFFER_PTR;
        Z_BUFFER_PTR = nullptr;
    }
    
    size_t max_z_levels = 8;
    Z_BUFFER_PTR = new uint32_t[NUM_TILES_X * NUM_TILES_Y * max_z_levels]();
    return 0;
}