#include <process.h>
#include <stdio.h>

#include "bedlam2.h"
#include "ddraw_func.h"
#include "dsound_func.h"
#include "ingame_music.h"
#include "main.h"
#include "timers.h"
#include "window.h"

DWORD GAME_THREAD_ID;
HANDLE HANDLE_GAME_THREAD;

#ifdef _DEBUG
int _cdecl main(int argc, char *argv[])
{
    HINSTANCE hInstance = NULL;
    HINSTANCE hPrevInstance = NULL;
    PSTR lpCmdLine = argv[1];
    // Needs for printf to console
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
#else
// 0044D6E8 Bedlam 1
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
#endif // _DEBUG

    SURFACE_IS_LOCKED = 1;
    int32_t error_code = init_window(hInstance, hPrevInstance, lpCmdLine);

    if (!error_code)
    {
        if (game_thread_create())
        {
            error_code = 4;
        }
        else
        {
            error_code = init_timer();
            if (!error_code)
            {
                error_code = static_cast<int>(message_dispatcher());
                timeKillEvent((UINT)TIMER_EVENT);
                timeEndPeriod((UINT)TIMER_RESOLUTION);
            }
            game_thread_exit();
        }
    }
    if (error_code == 1)
    {
        error_msgbox("Class error");
    }
    else if (error_code == 2)
    {
        error_msgbox("DDInit error");
    }
    else if (error_code == 3)
    {
        error_msgbox("SetVideoMode error");
    }
    else if (error_code == 4)
    {
        error_msgbox("Error starting thread");
    }
    else if (error_code == 5)
    {
        error_msgbox("Timer init error");
    }
    else if (error_code == 6)
    {
        error_msgbox("DSound init error");
    }
    else if (error_code == 1001)
    {
        error_msgbox("Please install DirectX");
    }
    else if (error_code == 1002)
    {
        error_msgbox("DirectX is already in use by another application");
    }
    else if (error_code == 1003)
    {
        error_msgbox("DirectDraw can not initialise a compatible display mode");
    }
    else if (error_code == 1004)
    {
        error_msgbox("DirectDraw initialization failed");
    }
    else if (error_code == 1010)
    {
        error_msgbox("No DirectDraw support possible with current display driver");
    }
    else
    {
        char buf[50];
        sprintf_s(buf, "Unknown error. Errorcode: %d", error_code);
        error_msgbox(buf);
    }

    free_win();
    return error_code;
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
