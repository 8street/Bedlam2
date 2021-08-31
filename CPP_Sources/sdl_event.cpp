#include <SDL.h>

#include "keyboard.h"
#include "main.h"
#include "mouse.h"
#include "palette.h"
#include "screenshot.h"
#include "sdl_event.h"
#include "sdl_timer.h"
#include "sdl_window.h"

int SDL_events()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            free_win();
            SDL_Quit();
            exit(0);
        }
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
        if (event.type == SDL_USEREVENT)
        {
            if (event.user.type == SDL_USEREVENT && event.user.code == 0)
            {
                if (NEW_PALETTE)
                {
                    GAME_WINDOW.set_palette((uint8_t *)NEW_PALETTE, 0, 256);
                    // GAME_WINDOW.redraw();
                    NEW_PALETTE = NULL;
                }
            }
            if (event.user.type == SDL_USEREVENT && event.user.code == 1)
            {
                int icon = WINDOW_CURSOR.get_cursor_img();
                if (icon != CURSOR_ICON)
                {
                    WINDOW_CURSOR.set_cursor_icon(CURSOR_ICON);
                }
                if (icon == ICON_WAIT)
                {
                    WINDOW_CURSOR.update_waiting_cursor();
                }
            }
        }
        if (event.type == SDL_KEYDOWN)
        {
            GAME_KEYBOARD.key_down(event.key.keysym.sym);
            if (event.key.keysym.sym == SDLK_PRINTSCREEN)
            {
                save_screenshot();
            }
            if (event.key.keysym.sym == SDLK_KP_PLUS)
            {
                GAME_WINDOW.increase_viewport_scale();
            }
            if (event.key.keysym.sym == SDLK_KP_MINUS)
            {
                GAME_WINDOW.decrease_viewport_scale();
            }
        }
        if (event.type == SDL_KEYUP)
        {
            GAME_KEYBOARD.key_up(event.key.keysym.sym);
        }

        switch (event.window.event)
        {
            case SDL_WINDOWEVENT_SHOWN:
                GAME_WINDOW.redraw();
                break;
            case SDL_WINDOWEVENT_HIDDEN:
                break;
            case SDL_WINDOWEVENT_EXPOSED:
                break;
            case SDL_WINDOWEVENT_MOVED:
                break;
            case SDL_WINDOWEVENT_RESIZED:
                GAME_WINDOW.resize_window(event.window.data1, event.window.data2);
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                break;
            case SDL_WINDOWEVENT_MINIMIZED:
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                GAME_WINDOW.set_window_pos_center();
                break;
            case SDL_WINDOWEVENT_RESTORED:
                GAME_WINDOW.redraw();
                break;
            case SDL_WINDOWEVENT_ENTER:
                break;
            case SDL_WINDOWEVENT_LEAVE:
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                GAME_WINDOW.redraw();
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
    }

    return 0;
}