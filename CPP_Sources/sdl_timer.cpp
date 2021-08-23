#include <SDL.h>
#include <iostream>

#include "bedlam2.h"
#include "mouse.h"
#include "sdl_timer.h"
#include "palette.h"

volatile int32_t WAITING_TIMER;
volatile int32_t GAME_UPDATE_TIMER;
static Uint32 sdl_timer_callback(Uint32 interval, void *param);
Timer GAME_TIMER;

void dos_sleep(uint32_t time)
{
    SDL_Delay(time);
}

Timer::Timer(int interval_ms)
    : m_interval_ms(interval_ms)
{
}

Timer::~Timer()
{
    SDL_RemoveTimer(m_timer_id);
    SDL_QuitSubSystem(SDL_INIT_TIMER);
}

int Timer::init(int interval_ms)
{
    m_interval_ms = interval_ms;
    return init();
}


int Timer::init()
{
    PALETTE_TIMER = 0;
    int ret_val = 0;

    if (SDL_Init(SDL_INIT_TIMER))
    {
        std::cout << "ERROR: Init SDL timer. " << SDL_GetError() << std::endl;
        ret_val |= -1;
    }

    m_timer_id = SDL_AddTimer(m_interval_ms, sdl_timer_callback, NULL);
    if (!m_timer_id)
    {
        std::cout << "ERROR: Add SDL timer. " << SDL_GetError() << std::endl;
        ret_val |= -1;
    }
    return ret_val;
}

Uint32 sdl_timer_callback(Uint32 interval, void *param)
{
    SDL_Event event;
    SDL_UserEvent userevent;
    SDL_UserEvent userevent2;
    static uint32_t cursor_timer;

    cursor_timer++;
    WAITING_TIMER++;
    GAME_UPDATE_TIMER++;
    level_clock();
    mouse_update();

    if (PALETTE_TIMER > 0)
    {
        PALETTE_TIMER--;
        palette_animation();
    }

    userevent.type = SDL_USEREVENT;
    userevent.code = 0;
    userevent.data1 = NULL;
    userevent.data2 = NULL;
    event.type = SDL_USEREVENT;
    event.user = userevent;
    SDL_PushEvent(&event);

    // animated cursor
    if ((cursor_timer & 7) == 0)
    {
        userevent2.type = SDL_USEREVENT;
        userevent2.code = 1;
        userevent2.data1 = NULL;
        userevent2.data2 = NULL;
        event.type = SDL_USEREVENT;
        event.user = userevent2;
        SDL_PushEvent(&event);
    }

    return (interval);
}