#include <SDL.h>
#include <iostream>

#include "bedlam2.h"
#include "mouse.h"
#include "sdl_timer.h"

SDL_TimerID my_timer_id;

volatile int32_t WAITING_TIMER;
volatile int32_t GAME_UPDATE_TIMER;

int init_timer()
{
    int ret_val = 0;
    ret_val = SDL_Init(SDL_INIT_TIMER) ;

    // for more soften performance 10 be changed to 9 
    my_timer_id = SDL_AddTimer(9, sdl_timer_callback, NULL);
    if (ret_val || !my_timer_id)
    {
        std::cout << "ERROR: Init timer \n";
        ret_val |= 1;
    }
    return ret_val;
}

Uint32 sdl_timer_callback(Uint32 interval, void *param)
{
    // SDL_Event event;
    // SDL_UserEvent userevent;

    ///* In this example, our callback pushes an SDL_USEREVENT event
    // into the queue, and causes our callback to be called again at the
    // same interval: */

    // userevent.type = SDL_USEREVENT;
    // userevent.code = 0;
    // userevent.data1 = NULL;
    // userevent.data2 = NULL;

    // event.type = SDL_USEREVENT;
    // event.user = userevent;

    // SDL_PushEvent(&event);

    WAITING_TIMER++;
    GAME_UPDATE_TIMER++;
    level_clock();
    mouse_update();
    return (interval);
}