#include <SDL.h>

#include "sdl_event.h"
#include "mouse.h"
#include "sdl_timer.h"

int SDL_events()
{

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            SDL_Quit();
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
                break;
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