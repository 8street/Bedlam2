#include <iostream>

#include "bedlam2.h"
#include "bedlam2_draw.h"
#include "sdl_event.h"
#include "sdl_window.h"
#include "timer.h"

Window GAME_WINDOW;

Window::Window()
{
}

Window::~Window()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    delete[] SCREEN_BUFFER_PTR;
    delete[] SIDEBAR_BUFFER_PTR;
    delete[] MAP_BUFFER_PTR;
    delete[] GAME_SCREEN_PTR;
    SCREEN_BUFFER_PTR = nullptr;
    SIDEBAR_BUFFER_PTR = nullptr;
    MAP_BUFFER_PTR = nullptr;
    GAME_SCREEN_PTR = nullptr;
    SDL_VideoQuit();
}

int Window::init()
{
    int ret_val = 0;

    if (SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "ERROR: init SDL video. " << SDL_GetError() << std::endl;
        ret_val |= 1;
        return ret_val;
    }

    SDL_DisplayMode DM;
    if (SDL_GetCurrentDisplayMode(0, &DM))
    {
        std::cout << "ERROR: get display mode. "<< SDL_GetError() << std::endl;
        ret_val |= 1;
    }
    int monitor_width = DM.w;
    int monitor_height = DM.h;
  
#ifdef _DEBUG
    const Resolution_settings &resolution_settings = m_options.get_resolution_settings(Resolution(1920, 1080));
#else
    const Resolution_settings &resolution_settings = m_options.get_resolution_settings(
        Resolution(monitor_width, monitor_height));
#endif
    m_game_width = resolution_settings.m_resolution.get_width();
    m_game_height = resolution_settings.m_resolution.get_height();
    m_window_width = m_game_width;
    m_window_height = m_game_height;

    int window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN;
    if (m_window_width == monitor_width && m_window_height == monitor_height)
    {
        window_flags = SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN;
    }
    m_window = SDL_CreateWindow(
        "Bedlam 2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_window_width, m_window_height, window_flags);
    if (!m_window)
    {
        std::cout << "ERROR: created window. " << SDL_GetError() << std::endl;
        ret_val |= 1;
        return ret_val;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (!m_renderer)
    {
        std::cout << "ERROR: created accelerated renderer is invalid. " << SDL_GetError() << std::endl;
        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_TARGETTEXTURE);
        if (!m_renderer)
        {
            std::cout << "ERROR: created software renderer is invalid. " << SDL_GetError() << std::endl;
            ret_val |= 1;
            return ret_val;
        }
    }

    SDL_SetWindowMinimumSize(m_window, ORIGINAL_GAME_WIDTH, ORIGINAL_GAME_HEIGHT);
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2"))
    {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            std::cout << "ERROR: Could't set render scale quality. " << SDL_GetError() << std::endl;
            ret_val |= 1;
        }
    }

    ret_val |= SDL_SetRenderTarget(m_renderer, NULL);
    ret_val |= SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
    ret_val |= SDL_RenderClear(m_renderer);
    SDL_RenderPresent(m_renderer);

    if (MAP_BUFFER_PTR == nullptr)
    {
        MAP_BUFFER_PTR = new uint8_t[ORIGINAL_GAME_WIDTH * ORIGINAL_GAME_HEIGHT]();
    }
    ret_val |= m_map.init(MAP_BUFFER_PTR, ORIGINAL_GAME_WIDTH, ORIGINAL_GAME_HEIGHT, MAP_WIDTH, MAP_HEIGHT);

    if (SIDEBAR_BUFFER_PTR == nullptr)
    {
        SIDEBAR_BUFFER_PTR = new uint8_t[ORIGINAL_GAME_WIDTH * ORIGINAL_GAME_HEIGHT]();
    }
    SIDEBAR_START_POS_X = m_game_width - SIDEBAR_WIDTH;
    ret_val |= m_sidebar.init(
        SIDEBAR_BUFFER_PTR, ORIGINAL_GAME_WIDTH, ORIGINAL_GAME_HEIGHT, SIDEBAR_WIDTH, SIDEBAR_HEIGHT,
        ORIGINAL_GAME_WIDTH - SIDEBAR_WIDTH);

    File game_pal("GAMEGFX/GAMEPAL.PAL");
    ret_val |= m_sidebar.set_palette(game_pal);
    ret_val |= m_map.set_palette(game_pal);

    ret_val |= m_sidebar.set_render_destination(SIDEBAR_START_POS_X, 0, SIDEBAR_WIDTH, SIDEBAR_HEIGHT);

    if (m_window_height > SIDEBAR_HEIGHT + MAP_HEIGHT)
    {
        // draw map bottom right
        ret_val |= m_map.set_render_destination(
            m_window_width - MAP_WIDTH, m_window_height - MAP_HEIGHT, MAP_WIDTH, MAP_HEIGHT);
    }
    else
    {
        // draw map top left
        ret_val |= m_map.set_render_destination(0, 0, MAP_WIDTH, MAP_HEIGHT);
    }

    ret_val |= reinit_screen_data(m_game_width, m_game_height);

    ret_val |= m_tiles.init_vars(resolution_settings);

    if (ret_val)
    {
        std::cout << "ERROR: init window." << std::endl;
    }

    return ret_val;
}

int Window::set_palette(uint8_t *pal_ptr, int offset, int num_entries)
{
    int ret_val = 0;
    ret_val |= m_screen.set_palette(pal_ptr, offset, num_entries);
    return ret_val;
}

int Window::clear_screen()
{
    int ret_val = 0;
    ret_val |= m_screen.clear();
    ret_val |= SDL_RenderClear(m_renderer);
    ret_val |= redraw();
    return ret_val;
}

int Window::fill_screen_surface(uint8_t *buffer)
{
    int ret_val = 0;
    ret_val |= m_screen.fill_screen_surface(buffer);
    return ret_val;
}

int Window::unlock_screen_surface() const
{
    int ret_val = 0;
    ret_val |= m_screen.unlock_surface();
    return ret_val;
}

int Window::redraw()
{
    int ret_val = 0;
    //Timer tim;

    if (game_is_playing)
    {
        // draw all screen when game level is playing
        m_screen.set_render_source(0, 0, m_screen.get_surface_width(), m_screen.get_surface_height());
        m_screen.set_render_destination(
            dead_screen_scale / 2, dead_screen_scale / 2, m_window_width - dead_screen_scale, m_window_height - dead_screen_scale);

    }
    else 
    {
        // upscale menu, map and armory screen  
        int menu_width = m_window_height * 4 / 3;
        int menu_start_pos_x = (m_window_width - menu_width) / 2;
        m_screen.set_render_source(0, 0, ORIGINAL_GAME_WIDTH, ORIGINAL_GAME_HEIGHT);
        m_screen.set_render_destination(menu_start_pos_x, 0, menu_width, m_window_height);
    }
    ret_val |= SDL_RenderCopy(m_renderer, m_screen.get_texture(), m_screen.get_render_source(), m_screen.get_render_destination());

    if (game_is_playing)
    {
        ret_val |= SDL_RenderCopy(m_renderer, m_sidebar.get_texture(), NULL, m_sidebar.get_render_destination());
        if (map_active)
        {
            ret_val |= SDL_RenderCopy(m_renderer, m_map.get_texture(), NULL, m_map.get_render_destination());
        }
    }

    //double elapsed = tim.elapsed();
    //elapsed = 0.0;
    SDL_RenderPresent(m_renderer);
    SDL_events();
    return ret_val;
}

int Window::clear_game_viewport()
{
    int ret_val = 0;
    ret_val |= m_screen.clear();
    return ret_val;
}

int Window::copy_screen_to_buffer(uint8_t *buffer_ptr)
{
    return m_screen.copy_surface_to_buffer(buffer_ptr);
}

uint8_t *Window::lock_and_get_surface_ptr()
{
    return m_screen.lock_and_get_surface_ptr();
}

uint8_t *Window::get_RGB_palette_ptr()
{
    return m_screen.get_RGB_palette_ptr();
}

int Window::get_window_height() const
{
    return m_window_height;
}

int Window::get_window_width() const
{
    return m_window_width;
}

int Window::get_game_height() const
{
    return m_game_height;
}

int Window::get_game_width() const
{
    return m_game_width;
}

int Window::draw_game_to_screen_buffer(uint8_t *game_screen_ptr, int32_t dead_screen_scale)
{
    uint8_t *source_ptr;
    uint8_t *destination_ptr;
    if (dead_screen_scale)
    {
        SDL_RenderClear(m_renderer);
    }
    else
    {
        int surf_width = m_screen.get_surface_width();
        source_ptr = &game_screen_ptr
                            [GAME_SCREEN_WIDTH * (((screen_y_pos & 31) + (screen_x_pos & 31u)) >> 1) // y
                            + 64 * GAME_SCREEN_WIDTH                                    // y offset (64 is tile size)
                            + 64                                                        // x offset
                            + (((screen_x_pos & 31) - (screen_y_pos & 31) + 32) & 63)]; // x
        destination_ptr = m_screen.lock_and_get_surface_ptr();
        int screen_line = m_game_height;
        do
        {
            memcpy(destination_ptr, source_ptr, m_game_width);
            source_ptr += GAME_SCREEN_WIDTH;
            destination_ptr += surf_width;
            --screen_line;
        } while (screen_line);
    }
    return 0;
}

int Window::resize_window(int new_width, int new_height)
{
    // size decreases
    if (new_height < m_window_height || new_width < m_window_width)
    {
        // keep aspect ratio
        if (new_width * m_window_height > new_height * m_window_width)
        {
            new_width = new_height * m_window_width / m_window_height;
        }
        else
        {
            new_height = new_width * m_window_height / m_window_width;
        }

    }
    // size increases
    else
    {
        // keep aspect ratio
        if (new_width * m_window_height > new_height * m_window_width)
        {
            new_height = new_width * m_window_height / m_window_width;
        }
        else
        {
            new_width = new_height * m_window_width / m_window_height;
        }
    }
    if (new_width < ORIGINAL_GAME_WIDTH || new_height < ORIGINAL_GAME_HEIGHT)
    {
        new_height = ORIGINAL_GAME_HEIGHT;
        new_width = new_height * m_game_width / m_game_height;
    }
    m_window_width = new_width;
    m_window_height = new_height;
    SDL_SetWindowSize(m_window, m_window_width, m_window_height);
    m_sidebar.set_render_destination(
        SIDEBAR_START_POS_X * m_window_width / m_game_width, 0, SIDEBAR_WIDTH * m_window_width / m_game_width, SIDEBAR_HEIGHT * m_window_height / m_game_height);
    if (m_game_height > SIDEBAR_HEIGHT + MAP_HEIGHT)
    {
        // draw map bottom right
        m_map.set_render_destination(
            (m_game_width - MAP_WIDTH) * m_window_width / m_game_width,
            (m_game_height - MAP_HEIGHT) * m_window_height / m_game_height, 
            MAP_WIDTH * m_window_width / m_game_width,
            MAP_HEIGHT * m_window_height / m_game_height);
    }
    else
    {
        // draw map top left
        m_map.set_render_destination(
            0, 0, MAP_WIDTH * m_window_width / m_game_width, MAP_HEIGHT * m_window_height / m_game_height);
    }
    return redraw();
}

int Window::reinit_screen_data(int new_width, int new_height)
{
    int ret_val = 0;
    m_game_width = new_width;
    m_game_height = new_height;

    ret_val |= m_screen.destroy();

    if (SCREEN_BUFFER_PTR)
    {
        delete[] SCREEN_BUFFER_PTR;
        SCREEN_BUFFER_PTR = nullptr;
    }
    SCREEN_BUFFER_PTR = new uint8_t[m_game_width * m_game_height]();

    SCREEN_SURFACE_WIDTH = m_game_width;
    SCREEN_SURFACE_HEIGHT = m_game_height;

    ret_val |= m_screen.init(SCREEN_BUFFER_PTR, m_game_width, m_game_height, m_game_width, m_game_height);
    ret_val |= reinit_game_screen_buffer(m_game_width, m_game_height);
    return ret_val;
}

int Window::reinit_game_screen_buffer(int new_width, int new_height)
{
    // Additional tile width needs to avoid black holes
    GAME_SCREEN_WIDTH = new_width + TILE_WIDTH * 5;

    int game_screen_height = new_height + TILE_HEIGHT * 20;
    // Additional tile height needs to avoid black holes and draw all Z levels in screen bottom
    GAME_SCREEN_SIZE = GAME_SCREEN_WIDTH * game_screen_height;

    // Limits to avoid copy sprite that exceeds screen size
    LIMIT_GAME_SCREEN_WIDTH = GAME_SCREEN_WIDTH - 2 * TILE_WIDTH;
    LIMIT_GAME_SCREEN_HEIGHT = game_screen_height - 2 * TILE_HEIGHT;
    if (GAME_SCREEN_PTR)
    {
        delete[] GAME_SCREEN_PTR;
        GAME_SCREEN_PTR = nullptr;
    }
    GAME_SCREEN_PTR = new uint8_t[GAME_SCREEN_SIZE]();
    return 0;
}

int Window::set_window_pos(int pos_x, int pos_y)
{
    if (pos_x >= 0 && pos_y >= 0)
    {
        SDL_SetWindowPosition(m_window, pos_x, pos_y);
        return 0;
    }
    return -1;
}

int Window::set_window_pos_center()
{
    SDL_SetWindowPosition(m_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    return 0;
}

SDL_Renderer *Window::get_renderer()
{
    return m_renderer;
}

int Window::increase_viewport_scale()
{
    const int width = m_screen.get_surface_width();
    const int height = m_screen.get_surface_height();
    m_viewport_scale_x += width >> 6;
    m_viewport_scale_y += height >> 6;
    if (m_viewport_scale_x > m_game_width / 2 || m_viewport_scale_y > m_game_height / 2)
    {
        m_viewport_scale_x = m_game_width / 2;
        m_viewport_scale_y = m_game_height / 2;
    }
    return m_screen.set_render_source(
        m_viewport_scale_x / 2, m_viewport_scale_y / 2, width - m_viewport_scale_x, height - m_viewport_scale_y);
}

int Window::decrease_viewport_scale()
{
    const int width = m_screen.get_surface_width();
    const int height = m_screen.get_surface_height();
    m_viewport_scale_x -= width >> 6;
    m_viewport_scale_y -= height >> 6;
    if (m_viewport_scale_x < 0 || m_viewport_scale_y < 0)
    {
        m_viewport_scale_x = 0;
        m_viewport_scale_y = 0;
    }
    return m_screen.set_render_source(
        m_viewport_scale_x / 2, m_viewport_scale_y / 2, width - m_viewport_scale_x, height - m_viewport_scale_y);
}