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
    NUM_TILES_X = settings.m_num_tiles_x;
    NUM_TILES_Y = settings.m_num_tiles_y;
    TILES_START_POS_X = settings.m_tiles_start_pos_x;
    TILES_START_POS_Y = settings.m_tiles_start_pos_y;

    //int original_start_pos_x = 304;
    //TILES_START_POS_X = original_start_pos_x;
    //int num_tiles_from_304_to_end = (settings.m_resolution.get_width() - TILES_START_POS_X + 4 * TILE_WIDTH) / (TILE_WIDTH / 2);
    //TILES_START_POS_Y = -num_tiles_from_304_to_end * TILE_HEIGHT / 2;

    //NUM_TILES_X = (settings.m_resolution.get_height() - TILES_START_POS_Y + 20 * TILE_HEIGHT) / (TILE_WIDTH / 2) + 12;
    //NUM_TILES_Y = (settings.m_resolution.get_height() - TILES_START_POS_Y + 20 * TILE_HEIGHT) / (TILE_WIDTH / 2) - 6;

    VETRICAL_TILE_LEVEL_OFFSET = TILES_START_POS_Y / TILE_HEIGHT + 9;

    NOT_VISIBLE_TILES_X = settings.m_not_visible_tiles_x;
    NOT_VISIBLE_TILES_Y = settings.m_not_visible_tiles_y;

    DISPLACE_SCREEN_X = (ORIGINAL_GAME_WIDTH - settings.m_resolution.get_width()) / 2;
    DISPLACE_SCREEN_Y = 0;

    if (Z_BUFFER_PTR)
    {
        delete[] Z_BUFFER_PTR;
        Z_BUFFER_PTR = nullptr;
    }
    
    uint32_t max_z_levels = 8;
    Z_BUFFER_PTR = new uint32_t[NUM_TILES_X * NUM_TILES_Y * max_z_levels]();
    return 0;
}