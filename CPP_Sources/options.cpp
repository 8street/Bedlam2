#include <algorithm>

#include "options.h"


Options::Options()
{ 
    // Add your resolution setting here in following format
    // m_resolutions_settings.push_back(Resolution_settings(Resolution(x,  y),  - screen resolution
    // num_tiles_x, num_tiles_y,                                                - number tiles
    // not_visible_tiles_x,  not_visible_tiles_x,                               - not visible tiles
    //                                                                            or visible tiles from robot pos to screen end
    //                                                                            I not sure
    // tiles_start_pos_x, tiles_start_pos_y,                                    - all tiles represent a rhombus with a beginning
    //                                                                            position at the top of rhombus
    // displace_origin_y));                                                     - displace vertical robot origin for correct robot 
    //                                                                            top part angle depending on mouse position
    //
    // You not need to change default tiles_start_pos_x value 304
    //
    // For correct working vertical offset
    // tiles_start_pos_y should be aliquot to value TILE_HEIGHT = 32

    m_resolutions_settings.push_back(Resolution_settings(Resolution(640,  480),  36,  36,  9,  9, 304, -256,  16));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(800,  600),  46,  36, 12, 12, 304, -256,  32));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(1024, 768),  56,  46, 15, 15, 304, -256,  62));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(1366, 768),  66,  56, 16, 18, 304, -448, -16));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(1440, 900),  70,  60, 16, 18, 304, -448,  28));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(1280, 1024), 70,  60, 16, 18, 304, -384, 132));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(1600, 900),  70,  60, 16, 18, 304, -448, -16));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(1600, 1200), 80,  65, 16, 26, 304, -512, 132));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(1920, 1080), 90,  70, 16, 32, 304, -736,  -6));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(1920, 1200), 100, 70, 16, 32, 304, -736,  56));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(2048, 1080), 100, 70, 16, 36, 304, -768, -32));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(2560, 1440), 120, 90, 16, 36, 304, -1056, 16));

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