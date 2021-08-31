#include <cmath>

#include "bedlam2_draw.h"
#include "resolutions.h"
#include "tiles.h"

bool operator==(const Resolution &r1, const Resolution &r2)
{
    return r1.m_width == r2.m_width && r1.m_height == r2.m_height;
}

bool operator<=(const Resolution &r1, const Resolution &r2)
{
    return r1.m_height * r1.m_width <= r2.m_height * r2.m_width;
}

bool operator>(const Resolution &r1, const Resolution &r2)
{
    return !(r1 <= r2);
}

bool operator>=(const Resolution &r1, const Resolution &r2)
{
    return r1.m_height * r1.m_width >= r2.m_height * r2.m_width;
}

bool operator<(const Resolution &r1, const Resolution &r2)
{
    return !(r1 >= r2);
}

Resolution &Resolution::operator=(const Resolution &res)
{
    m_width = res.m_width;
    m_height = res.m_height;
    return *this;
}

Resolution Resolution::operator-() const
{
    return Resolution(-m_width, -m_height);
}

Resolution &Resolution::operator+(const Resolution &res)
{
    m_width += res.m_width;
    m_height += res.m_height;
    return *this;
}

Resolution operator-(const Resolution &r1, const Resolution &r2)
{
    return Resolution(r1.m_width - r2.m_width, r1.m_height - r2.m_height);
}

Resolution Resolution::abs(const Resolution &r)
{
    return Resolution(std::abs(r.m_width), std::abs(r.m_height));
}

int Resolution::get_width() const
{
    return m_width;
}

int Resolution::get_height() const
{
    return m_height;
}

Resolution_settings::Resolution_settings()
{
}

Resolution_settings::Resolution_settings(
    const Resolution &res, int num_tiles_x, int num_tiles_y, int not_visible_tiles_x, int not_visible_tiles_y,
    int tiles_start_pos_x, int tiles_start_pos_y, int displace_origin_y)
    : m_resolution(res)
    , m_num_tiles_x(num_tiles_x)
    , m_num_tiles_y(num_tiles_y)
    , m_not_visible_tiles_x(not_visible_tiles_x)
    , m_not_visible_tiles_y(not_visible_tiles_y)
    , m_tiles_start_pos_x(tiles_start_pos_x)
    , m_tiles_start_pos_y(tiles_start_pos_y)
    , m_displace_origin_y(displace_origin_y)
{
}

Resolution_settings::~Resolution_settings()
{
}

bool operator<(const Resolution_settings &rs1, const Resolution_settings &rs2)
{
    return rs1.m_resolution < rs2.m_resolution;
}