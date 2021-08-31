#pragma once

class Resolution
{
public:
    Resolution(int width = 0, int height = 0)
        : m_width(width)
        , m_height(height){};
    Resolution(const Resolution &res)
        : m_width(res.m_width)
        , m_height(res.m_height){};
    friend bool operator==(const Resolution &r1, const Resolution &r2);
    friend bool operator<=(const Resolution &r1, const Resolution &r2);
    friend bool operator>(const Resolution &r1, const Resolution &r2);
    friend bool operator>=(const Resolution &r1, const Resolution &r2);
    friend bool operator<(const Resolution &r1, const Resolution &r2);
    Resolution &operator=(const Resolution &res);
    Resolution operator-() const;
    Resolution &operator+(const Resolution &res);
    friend Resolution operator-(const Resolution &r1, const Resolution &r2);
    static Resolution abs(const Resolution &r);
    int get_width() const;
    int get_height() const;

private:
    int m_width;
    int m_height;
};

class Resolution_settings
{
public:
    Resolution_settings();
    Resolution_settings(
        const Resolution &res, int num_tiles_x = 0, int num_tiles_y = 0, int not_visible_tiles_x = 0,
        int not_visible_tiles_y = 0, int tiles_start_pos_x = 0, int tiles_start_pos_y = 0, int displace_origin_y = 0);
    ~Resolution_settings();
    friend bool operator<(const Resolution_settings &rs1, const Resolution_settings &rs2);

    Resolution m_resolution;
    int m_vertical_tile_offset = 0;
    int m_num_tiles_x = 0;
    int m_num_tiles_y = 0;
    int m_not_visible_tiles_x = 0;
    int m_not_visible_tiles_y = 0;
    int m_tiles_start_pos_x = 0;
    int m_tiles_start_pos_y = 0;
    int m_displace_origin_y = 0;

private:
};
