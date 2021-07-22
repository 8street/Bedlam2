#include "timers.h"
#include "bedlam2.h"
#include "ddraw_func.h"
#include "mouse.h"
#include "palette.h"

uint32_t TIMER_RESOLUTION;
uint32_t TIMER_EVENT;



// 0044DE58 Bedlam 1
void timer_callback(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    struct tagPOINT cursor; // [esp+0h] [ebp-8h] BYREF

    timer_update();

    if (UPDATE_CURSOR_BY_TIMER == 1 && !IS_BLITTING)
    {
        GetCursorPos(&cursor);
        blit_cursor(cursor.x, cursor.y);
    }
}

// 0041BFB6 Bedlam 1
void timer_update()
{
    static uint32_t cursor_timer;

    cursor_timer++;
    increment_timers();
    mouse_update();

    // animated cursor
    if (CURSOR_ICON >= ICON_WAIT && CURSOR_ICON < 152 && (cursor_timer & 7) == 0)
    {
        ++CURSOR_ICON;
        if (CURSOR_ICON == 151)
        {
            CURSOR_ICON = ICON_WAIT;
        }
        set_cursor_icon(CURSOR_ICON);
    }
}

// 00402B0C Bedlam 1
void increment_timers()
{
   ///* WAITING_TIMER++;
   // GAME_UPDATE_TIMER++;*/
   // level_clock();
   // if (PALETTE_TIMER)
   // {
   //     palette_animation();
   // }
}

void timer_deinit()
{
    if (TIMER_EVENT)
    {
        timeKillEvent((UINT)TIMER_EVENT);
        timeEndPeriod((UINT)TIMER_RESOLUTION);
        TIMER_EVENT = 0;
        Sleep(500);
    }
}

void dos_sleep(uint32_t time)
{
    Sleep(time);
}