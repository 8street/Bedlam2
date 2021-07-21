#include <process.h>
#include <stdio.h>

#include "bedlam2.h"
#include "ddraw_func.h"
#include "dsound_func.h"
#include "ingame_music.h"
#include "main.h"
#include "timers.h"
#include "window.h"

#include "sdl_draw.h"
#include "sdl_event.h"
#include "sdl_thread.h"
#include "sdl_timer.h"
#include "sdl_window.h"

DWORD GAME_THREAD_ID;
HANDLE HANDLE_GAME_THREAD;

int main(int argc, char *argv[])
{
    init_video();
    init_dsound();
    init_timer();

    return main_(0, NULL, NULL);
}

// 0044D9C0 Bedlam 1
int16_t game_thread_create()
{
    int16_t retVal; // ax

    // thread_flag_ = 0;
    HANDLE_GAME_THREAD = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)game_thread_start_func, NULL, 0, &GAME_THREAD_ID);
    retVal = GAME_THREAD_ID == -1;
    return retVal;
}

// 0044DA1C Bedlam 1
void game_thread_exit()
{
    if (GAME_THREAD_ID != -1)
    {
        SuspendThread(HANDLE_GAME_THREAD);
        TerminateThread(HANDLE_GAME_THREAD, 0);
        Sleep(500);
        GAME_THREAD_ID = -1;
    }
}

// 0044DEA0 Bedlam 1
void game_thread_start_func()
{
    int32_t exit_code = main_(0, NULL, NULL);
    GAME_THREAD_ID = -1;
    SendMessageA(WINDOW_HWND, WM_DESTROY, 0, 0);
}

void free_win()
{
    stop_wav_music();
    timer_deinit();
    ddraw_deinit();
    dsound_deinit();
}
