#pragma once
#include <vector>

#include "resolutions.h"
class Options
{
public:
    Options();
    ~Options();
    const Resolution_settings &get_resolution_settings(const Resolution &res) const;

private:
    const Resolution_settings &get_nearest_resolution_setting(const Resolution &res) const;
    std::vector<Resolution_settings> m_resolutions_settings;
};
