#include "tiles.h"
#include "bedlam2_draw.h"

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
    NUM_TILES_X = settings.m_num_tiles_x;
    NUM_TILES_Y = settings.m_num_tiles_y;
    TILES_START_POS_X = settings.m_tiles_start_pos_x;
    TILES_START_POS_Y = settings.m_tiles_start_pos_y;

    VETRICAL_TILE_LEVEL_OFFSET = TILES_START_POS_Y / TILE_HEIGHT + 9;

    NOT_VISIBLE_TILES_X = settings.m_not_visible_tiles_x;
    NOT_VISIBLE_TILES_Y = settings.m_not_visible_tiles_y;

    DISPLACE_SCREEN_X = (ORIGINAL_GAME_WIDTH - settings.m_resolution.get_width()) / 2;
    DISPLACE_SCREEN_Y = 0;

    DISPLACE_ROBOT_ORIGIN_Y = settings.m_displace_origin_y;

    if (Z_BUFFER_PTR)
    {
        delete[] Z_BUFFER_PTR;
        Z_BUFFER_PTR = nullptr;
    }

    uint32_t max_z_levels = 8;
    Z_BUFFER_PTR = new uint32_t[NUM_TILES_X * NUM_TILES_Y * max_z_levels]();
    return 0;
}