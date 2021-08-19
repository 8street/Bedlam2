#include <algorithm>

#include "options.h"


Options::Options()
{ 
    m_resolutions_settings.push_back(Resolution_settings(Resolution(640, 480), 2, 36, 36, 9, 9));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(800, 600), 2, 36, 36, 9, 9));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(1024, 768), 2, 36, 36, 9, 9));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(1280, 1024), 2, 36, 36, 9, 9));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(1600, 1200), 2, 36, 36, 9, 9));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(1920, 1080), 2, 36, 36, 9, 9));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(1920, 1200), 2, 36, 36, 9, 9));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(2048, 1080), 2, 36, 36, 9, 9));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(3840, 2160), 2, 36, 36, 9, 9));
    m_resolutions_settings.push_back(Resolution_settings(Resolution(7680, 4320), 2, 36, 36, 9, 9));

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