
#include "render_position.h"
#include "bedlam2.h"

Render_position::Render_position(bool enable)
{
}

Render_position::~Render_position()
{
}

const SDL_Rect *Render_position::get_render_source() const
{
    return &m_render_source;
}
const SDL_Rect *Render_position::get_render_destination() const
{
    return &m_render_destination;
}

int Render_position::set_render_source(int x, int y, int w, int h)
{
    m_render_source.x = x;
    m_render_source.y = y;
    m_render_source.w = w;
    m_render_source.h = h;
    return 0;
}
int Render_position::set_render_destination(int x, int y, int w, int h)
{
    m_render_destination.x = x;
    m_render_destination.y = y;
    m_render_destination.w = w;
    m_render_destination.h = h;
    return 0;
}