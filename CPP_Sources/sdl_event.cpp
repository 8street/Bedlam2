#include "sdl_event.h"
#include "keyboard.h"
#include "main.h"
#include "mouse.h"
#include "palette.h"
#include "screenshot.h"
#include "sdl_timer.h"
#include "sdl_window.h"

int SDL_events()
{
    int ret_val = 0;
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ret_val |= quit_events(event);
        ret_val |= mouse_events(event);
        ret_val |= palette_events(event);
        ret_val |= cursor_events(event);
        ret_val |= keyboard_events(event);
        ret_val |= window_events(event);
    }
    return ret_val;
}

int quit_events(const SDL_Event &event)
{
    if (event.type == SDL_QUIT)
    {
        free_win();
        SDL_Quit();
        exit(0);
    }
    return 0;
}

int mouse_events(const SDL_Event &event)
{
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            mouse_buttons(0, 0);
        }
        if (event.button.button == SDL_BUTTON_RIGHT)
        {
            mouse_buttons(1, 0);
        }
    }
    if (event.type == SDL_MOUSEBUTTONUP)
    {
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            mouse_buttons(0, 1);
        }
        if (event.button.button == SDL_BUTTON_RIGHT)
        {
            mouse_buttons(1, 1);
        }
    }
    return 0;
}

int palette_events(const SDL_Event &event)
{
    int ret_val = 0;
    if (event.type == SDL_USEREVENT)
    {
        if (event.user.type == SDL_USEREVENT && event.user.code == 0)
        {
            if (NEW_PALETTE)
            {
                ret_val |= GAME_WINDOW.set_palette((uint8_t *)NEW_PALETTE, 0, 256);
                // GAME_WINDOW.redraw();
                NEW_PALETTE = NULL;
            }
        }
    }
    return ret_val;
}

int cursor_events(const SDL_Event &event)
{
    int ret_val = 0;
    if (event.type == SDL_USEREVENT)
    {
        if (event.user.type == SDL_USEREVENT && event.user.code == 1)
        {
            const int icon = WINDOW_CURSOR.get_cursor_img();
            if (icon != CURSOR_ICON)
            {
                ret_val |= WINDOW_CURSOR.set_cursor_icon(CURSOR_ICON);
            }
            if (icon == ICON_WAIT)
            {
                ret_val |= WINDOW_CURSOR.update_waiting_cursor();
            }
        }
    }
    return ret_val;
}

int keyboard_events(const SDL_Event &event)
{
    int ret_val = 0;
    if (event.type == SDL_KEYDOWN)
    {
        ret_val |= GAME_KEYBOARD.key_down(event.key.keysym.sym);
        if (event.key.keysym.sym == SDLK_PRINTSCREEN)
        {
            ret_val |= save_screenshot();
        }
        if (event.key.keysym.sym == SDLK_KP_PLUS)
        {
            ret_val |= GAME_WINDOW.increase_viewport_scale();
        }
        if (event.key.keysym.sym == SDLK_KP_MINUS)
        {
            ret_val |= GAME_WINDOW.decrease_viewport_scale();
        }
    }
    if (event.type == SDL_KEYUP)
    {
        ret_val |= GAME_KEYBOARD.key_up(event.key.keysym.sym);
    }
    return ret_val;
}

int window_events(const SDL_Event &event)
{
    int ret_val = 0;
    switch (event.window.event)
    {
        case SDL_WINDOWEVENT_SHOWN:
            ret_val |= GAME_WINDOW.redraw();
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            break;
        case SDL_WINDOWEVENT_MOVED:
            break;
        case SDL_WINDOWEVENT_RESIZED:
            ret_val |= GAME_WINDOW.resize_window(event.window.data1, event.window.data2);
            break;
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            ret_val |= GAME_WINDOW.set_window_pos_center();
            break;
        case SDL_WINDOWEVENT_RESTORED:
            ret_val |= GAME_WINDOW.redraw();
            break;
        case SDL_WINDOWEVENT_ENTER:
            break;
        case SDL_WINDOWEVENT_LEAVE:
            break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            ret_val |= GAME_WINDOW.redraw();
            break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            break;
        case SDL_WINDOWEVENT_CLOSE:
            break;
#if SDL_VERSION_ATLEAST(2, 0, 5)
        case SDL_WINDOWEVENT_TAKE_FOCUS:
            break;
        case SDL_WINDOWEVENT_HIT_TEST:
            break;
#endif
        default:
            // SDL_Log("Window %d got unknown event %d", event.window.windowID, event.window.event);
            break;
    }
    return ret_val;
}