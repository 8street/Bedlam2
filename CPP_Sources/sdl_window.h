#pragma once
#include <SDL.h>
#include <cstdint>

#define SIDEBAR_WIDTH 160

class Window
{
public:
    Window();
    ~Window();

    int init();

    int set_palette(uint8_t *pal_ptr, int offset, int num_entries);
    int clear_screen();
    int fill_screen_surface(uint8_t *buffer_ptr);
    int unlock_screen_surface() const;
    int redraw();
    int clear_game_viewport();
    int copy_screen_to_buffer(uint8_t *buffer_ptr);
    uint8_t *lock_and_get_surface_ptr();
    uint8_t *get_RGB_palette_ptr();
    int get_window_height() const;
    int get_window_width() const;
    int get_game_height() const;
    int get_game_width() const;
    int draw_game_or_map(uint8_t *game_screen_ptr, int32_t map_active, int32_t dead_screen_scale);
    int resize_window(int new_width, int new_height);
    int set_window_pos(int pos_x, int pos_y);
    int set_window_pos_center();
    SDL_Renderer *get_renderer();
    int increase_viewport_scale();
    int decrease_viewport_scale();

private:
    int dead_screen_scaler(uint8_t *game_screen_ptr, int32_t dead_screen_scale);
    int fill_screen_texture_from_surface();

    int m_window_width = 0;
    int m_window_height = 0;
    int m_game_width = 0;
    int m_game_height = 0;
    SDL_Window *m_window = nullptr;
    SDL_Renderer *m_renderer = nullptr;
    SDL_Surface *m_screen_surface = nullptr;
    SDL_Texture *m_screen_texture = nullptr;
    bool m_must_lock_surface = false;
    SDL_Rect m_source_viewport_rect = { 0 };
    SDL_Rect m_destination_viewport_rect = { 0 };
    int m_viewport_scale = 0;
};

extern "C" uint8_t *SCREEN_BUFFER_PTR;
extern uint8_t GAME_SCREEN_PTR[409600];
extern "C" volatile uint32_t SCREEN_SURFACE_WIDTH;
extern volatile uint32_t SCREEN_SURFACE_HEIGHT;

extern Window GAME_WINDOW;