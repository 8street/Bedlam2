#include "bedlam2_draw.h"
#include "bedlam2.h"
#include "sdl_event.h"
#include "sdl_timer.h"
#include "sdl_window.h"

uint8_t *SIDEBAR_BUFFER_PTR = nullptr;
uint8_t *MAP_BUFFER_PTR = nullptr;
uint8_t *SCREEN_BUFFER_PTR = nullptr;
uint8_t GAME_SCREEN_PTR[409600];
volatile uint32_t SCREEN_SURFACE_WIDTH;
volatile uint32_t SCREEN_SURFACE_HEIGHT;

void clear_screen()
{
    GAME_WINDOW.clear_screen();
}

void copy_buffer_to_screen_and_unlock(uint8_t *buffer)
{
    GAME_WINDOW.fill_screen_surface(buffer);
    GAME_WINDOW.unlock_screen_surface();
}

void redraw()
{
    GAME_WINDOW.redraw();
}

int unlock_surface_and_wait(int time_to_waiting)
{
    GAME_WINDOW.unlock_screen_surface();
    WAITING_TIMER = 0;
    while (time_to_waiting > WAITING_TIMER)
    {
        SDL_events();
    }
    return 0;
}

void redraw_copy_buff_to_screen_and_unlock(uint8_t *screen_buf)
{
    GAME_WINDOW.redraw();
    GAME_WINDOW.fill_screen_surface(screen_buf);
}

// Copy map or game screen in screen surface
void draw_map_and_game_screen()
{
    GAME_WINDOW.draw_game_or_map(game_screen_ptr, map_active, dead_screen_scale);
}

void copy_screen_to_buffer(uint8_t *buffer_ptr)
{
    GAME_WINDOW.copy_screen_to_buffer(buffer_ptr);
}

void get_screen_buffer_ptr()
{
    GAME_WINDOW.lock_and_get_surface_ptr();
    // SCREEN_BUFFER_PTR already map with SCREEN_SURFACE->pixels
}