#include <algorithm>

#include "options.h"


Options::Options()
{ 
    m_resolutions_settings.push_back(Resolution_settings(Resolution(640,  480),    2, 36,  36,  9,  9, 304, -256));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(800,  600),    1, 46,  36, 12, 12, 304, -256));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(1024, 768),    1, 56,  46, 15, 15, 304, -256));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(1280, 1024),  -3, 70,  50, 15, 15, 304, -384));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(1600, 1200),  -7, 80,  65, 15, 15, 304, -512));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(1920, 1080), -13, 90,  70, 15, 15, 304, -720));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(1920, 1200), -13, 100, 70, 15, 15, 304, -720));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(2048, 1080), -15, 100, 70, 15, 15, 304, -720));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(2560, 1440), -21, 120, 90, 15, 15, 304, -960));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(3840, 2160), -13, 100, 70, 15, 15, 304, -768));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(7680, 4320),   2, 36,  36,  9,  9, 304, -256));

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