#include <SDL.h>

#include "sdl_event.h"
#include "mouse.h"
#include "sdl_timer.h"
#include "palette.h"
#include "keyboard.h"
#include "sdl_draw.h"
#include "screenshot.h"

int SDL_events()
{
    SDL_Event event;

    SDL_DisplayMode DM;
    int monitor_width;
    int monitor_height;
    int new_width;
    int new_height;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
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
                    sdl_set_palette((uint8_t *)NEW_PALETTE, 0, 256);
                    redraw();
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
        }
        if (event.type == SDL_KEYUP)
        {
            GAME_KEYBOARD.key_up(event.key.keysym.sym);
        }

        switch (event.window.event)
        {
            case SDL_WINDOWEVENT_SHOWN:
                redraw();
                // SDL_Log("Window %d shown", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_HIDDEN:
                // SDL_Log("Window %d hidden", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_EXPOSED:
                // SDL_Log("Window %d exposed", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_MOVED:
                // SDL_Log("Window %d moved to %d,%d",
                // event->window.windowID, event->window.data1,
                // event->window.data2);
                break;
            case SDL_WINDOWEVENT_RESIZED:
                // SDL_Log("Window %d resized to %dx%d",
                // event->window.windowID, event->window.data1,
                // event->window.data2);
                new_width = event.window.data1;
                new_height = event.window.data2;
                if (new_height < WINDOW_HEIGHT || new_width < WINDOW_WIDTH)
                {
                    if (new_width * 3 > new_height * 4)
                    {
                        new_width = new_height * 4 / 3;
                    }
                    else
                    {
                        new_height = new_width * 3 / 4;
                    }
                }
                else
                {
                    if (new_width * 3 > new_height * 4)
                    {
                        new_height = new_width * 3 / 4;
                    }
                    else
                    {
                        new_width = new_height * 4 / 3;
                    }
                }
                WINDOW_HEIGHT = new_height;
                WINDOW_WIDTH = new_width;
                SDL_SetWindowSize(WINDOW, WINDOW_WIDTH, WINDOW_HEIGHT);
                redraw();
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                // SDL_DisplayMode DM;
                // SDL_GetCurrentDisplayMode(0, &DM);
                // SDL_Log("Window %d size changed to %dx%d",
                // event->window.windowID, event->window.data1,
                // event->window.data2);
                //SDL_SetWindowSize(WINDOW, WINDOW_WIDTH, WINDOW_HEIGHT);
                //redraw();
                break;
            case SDL_WINDOWEVENT_MINIMIZED:
                // SDL_Log("Window %d minimized", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                SDL_GetCurrentDisplayMode(0, &DM);
                monitor_width = DM.w;
                monitor_height = DM.h;
                SDL_SetWindowPosition(WINDOW, (monitor_width - WINDOW_WIDTH) / 2, (monitor_height - WINDOW_HEIGHT) / 2);
                // SDL_Log("Window %d maximized", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_RESTORED:
                redraw();
                // SDL_Log("Window %d restored", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_ENTER:
                // SDL_Log("Mouse entered window %d",
                // event->window.windowID);
                break;
            case SDL_WINDOWEVENT_LEAVE:
                // SDL_Log("Mouse left window %d", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                redraw();
                // SDL_Log("Window %d gained keyboard focus",
                // event->window.windowID);
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                // SDL_Log("Window %d lost keyboard focus",
                // event->window.windowID);
                break;
            case SDL_WINDOWEVENT_CLOSE:
                // SDL_Log("Window %d closed", event->window.windowID);
                break;
#if SDL_VERSION_ATLEAST(2, 0, 5)
            case SDL_WINDOWEVENT_TAKE_FOCUS:
                // SDL_Log("Window %d is offered a focus", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_HIT_TEST:
                // SDL_Log("Window %d has a special hit test", event->window.windowID);
                break;
#endif
            default:
                // SDL_Log("Window %d got unknown event %d",
                // event->window.windowID, event->window.event);
                break;
        }
    }

    return 0;
}