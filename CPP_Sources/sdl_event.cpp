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
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    KEY_ESC = 1;
                    break;
                case SDLK_1:
                    KEY_1 = 1;
                    break;
                case SDLK_2:
                    KEY_2 = 1;
                    break;
                case SDLK_3:
                    KEY_3 = 1;
                    break;
                case SDLK_4:
                    KEY_4 = 1;
                    break;
                case SDLK_5:
                    KEY_5 = 1;
                    break;
                case SDLK_6:
                    KEY_6 = 1;
                    break;
                case SDLK_7:
                    KEY_7 = 1;
                    break;
                case SDLK_F1:
                    KEY_F1 = 1;
                    break;
                case SDLK_F2:
                    KEY_F2 = 1;
                    break;
                case SDLK_F3:
                    KEY_F3 = 1;
                    break;
                case SDLK_p:
                    KEY_P = 1;
                    break;
                case SDLK_m:
                case SDLK_SPACE:
                    KEY_M = 1;
                    break;
                case SDLK_PRINTSCREEN:
                    save_screenshot();
                    break;
            }
        }
        if (event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    KEY_ESC = 0;
                    break;
                case SDLK_1:
                    KEY_1 = 0;
                    break;
                case SDLK_2:
                    KEY_2 = 0;
                    break;
                case SDLK_3:
                    KEY_3 = 0;
                    break;
                case SDLK_4:
                    KEY_4 = 0;
                    break;
                case SDLK_5:
                    KEY_5 = 0;
                    break;
                case SDLK_6:
                    KEY_6 = 0;
                    break;
                case SDLK_7:
                    KEY_7 = 0;
                    break;
                case SDLK_F1:
                    KEY_F1 = 0;
                    break;
                case SDLK_F2:
                    KEY_F2 = 0;
                    break;
                case SDLK_F3:
                    KEY_F3 = 0;
                    break;
                case SDLK_p:
                    KEY_P = 0;
                    break;
                case SDLK_m:
                case SDLK_SPACE:
                    KEY_M = 0;
                    break;
            }
        }

        switch (event.window.event)
        {
            case SDL_WINDOWEVENT_SHOWN:
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
                WINDOW_WIDTH = event.window.data1;
                WINDOW_HEIGHT = event.window.data2;
                if (WINDOW_WIDTH * 3 > WINDOW_HEIGHT * 4)
                {
                    WINDOW_WIDTH = WINDOW_HEIGHT * 4 / 3;
                }
                else
                {
                    WINDOW_HEIGHT = WINDOW_WIDTH * 3 / 4;
                }
                break;
                redraw();
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                // SDL_DisplayMode DM;
                // SDL_GetCurrentDisplayMode(0, &DM);
                // SDL_Log("Window %d size changed to %dx%d",
                // event->window.windowID, event->window.data1,
                // event->window.data2);
                break;
            case SDL_WINDOWEVENT_MINIMIZED:
                // SDL_Log("Window %d minimized", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
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