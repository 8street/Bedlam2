#include <algorithm>

#include "options.h"


Options::Options()
{ 
    // Add your resolution setting here in following format
    // m_resolutions_settings.push_back(Resolution_settings(Resolution(x,  y),  - screen resolution
    // vertical_tile_offset,                                                    - tiles offset in Z levels 
    // num_tiles_x, num_tiles_y,                                                - number tiles
    // not_visible_tiles_x,  not_visible_tiles_x,                               - not visible tiles
    //                                                                            or visible tiles from robot pos to screen end
    //                                                                            I not sure
    // tiles_start_pos_x, tiles_start_pos_y));                                  - all tiles represent a rhombus with a beginning
    //                                                                            position at the top of rhombus

    // You not need to change default tiles_start_pos_x value 304
    
    // For correct working vertical_tile_offset variable
    // tiles_start_pos_y should be aliquot to value TILE_HEIGHT = 32

    m_resolutions_settings.push_back(Resolution_settings(Resolution(640,  480),    2, 36,  36,  9,  9, 304, -256));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(800,  600),    1, 46,  36, 12, 12, 304, -256));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(1024, 768),    1, 56,  46, 15, 15, 304, -256));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(1280, 1024),  -3, 70,  50, 15, 15, 304, -384));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(1600, 1200),  -7, 80,  65, 15, 15, 304, -512));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(1920, 1080), -13, 90,  70, 15, 15, 304, -736));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(1920, 1200), -13, 100, 70, 15, 15, 304, -736));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(2048, 1080), -15, 100, 70, 15, 15, 304, -736));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(2560, 1440), -21, 120, 90, 15, 15, 304, -960));

    std::sort(m_resolutions_settings.begin(), m_resolutions_settings.end());
}

Options::~Options()
{
}

const Resolution_settings &Options::get_resolution_settings(const Resolution &res) const
{
    return get_nearest_resolution_setting(res);
}

const Resolution_settings &Options::get_nearest_resolution_setting(const Resolution &res) const
{
    Resolution_settings tmp_setting(res);
    auto const it = std::lower_bound(m_resolutions_settings.begin(), m_resolutions_settings.end(), tmp_setting);
    if (it == m_resolutions_settings.end())
    {
        return *(it - 1);
    }
    return *it;
}