#include <SDL.h>
#include <iostream>

#include "sdl_timer.h"
#include "bedlam2.h"
#include "mouse.h"
SDL_TimerID my_timer_id;

volatile int32_t WAITING_TIMER;
volatile int32_t GAME_UPDATE_TIMER;

int init_timer()
{
    if (SDL_Init(SDL_INIT_TIMER) < 0)
    {
        std::cout << "ERROR: Init timer \n";
        return 1;
    }
    my_timer_id = SDL_AddTimer(10, sdl_timer_callback, NULL);
    return 0;
}

Uint32 sdl_timer_callback(Uint32 interval, void *param)
{
    //SDL_Event event;
    //SDL_UserEvent userevent;

    ///* In this example, our callback pushes an SDL_USEREVENT event
    //into the queue, and causes our callback to be called again at the
    //same interval: */

    //userevent.type = SDL_USEREVENT;
    //userevent.code = 0;
    //userevent.data1 = NULL;
    //userevent.data2 = NULL;

    //event.type = SDL_USEREVENT;
    //event.user = userevent;

    //SDL_PushEvent(&event);

    WAITING_TIMER++;
    GAME_UPDATE_TIMER++;
    level_clock();
    mouse_update();
    return (interval);
}