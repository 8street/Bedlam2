#include <Windows.h>
#include <WinUser.h>

#include "ddraw_func.h"
#include "dsound_func.h"
#include "helper.h"
#include "keyboard.h"
#include "main.h"
#include "mouse.h"
#include "screenshot.h"
#include "window.h"

HWND WINDOW_HWND;

uint8_t APPLICATION_ACTIVE;
uint8_t NEED_EXIT;
int32_t WINDOW_POS_X;
int32_t WINDOW_POS_Y;
int32_t WINDOW_WIDTH;
int32_t WINDOW_HEIGHT;

// 0044D320 Bedlam 1
int init_window(HINSTANCE hwnd, HINSTANCE prev_hwnd, LPSTR lp_cmd_line)
{
    //FULLSCREEN = 0;
    //USE_VIDEOMEMORY = 1;

    //int i = 0;
    //if (lp_cmd_line)
    //{
    //    if (strstr(lp_cmd_line, "-f") != NULL || strstr(lp_cmd_line, "-F") != NULL)
    //    {
    //        FULLSCREEN = 1;
    //    }
    //}

    //if (!FULLSCREEN)
    //{
    //    HDC dc = GetDC(NULL);
    //    if ((GetDeviceCaps(dc, RASTERCAPS) & RC_PALETTE) == 0)
    //    {
    //        if (MessageBoxA(NULL, "256 colour mode needed\nClick 'OK' to run full-screen", "Problem", 0x11) == 2)
    //        {
    //            exit(0);
    //        }
    //        FULLSCREEN = 1;
    //    }
    //}

    //HINSTANCE base_addres = hwnd;
    //const char class_name[] = "Bedlam2";

    //if (!prev_hwnd)
    //{
    //    WNDCLASSEXA classex;
    //    classex.cbSize = sizeof(classex);
    //    classex.style = CS_VREDRAW | CS_HREDRAW;
    //    // if (dword_4EDECE >> 16 == 1)
    //    //    classex.style = 11;
    //    classex.hInstance = hwnd;
    //    classex.lpfnWndProc = (WNDPROC)WindowProc;
    //    classex.cbClsExtra = 0;
    //    classex.cbWndExtra = 0;
    //    classex.hIcon = LoadIconA(hwnd, (LPCSTR)0x7D0);
    //    classex.hIconSm = LoadIconA(hwnd, (LPCSTR)0x7D1);
    //    classex.hCursor = LoadCursorA(0, (LPCSTR)0x7F00);
    //    classex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    //    classex.lpszClassName = class_name;
    //    classex.lpszMenuName = 0;
    //    if (!RegisterClassExA(&classex))
    //    {
    //        return 1;
    //    }
    //}

    //int32_t display_width = GetSystemMetrics(SM_CXSCREEN);
    //int32_t display_height = GetSystemMetrics(SM_CYSCREEN);

    //uint32_t dwStyle = 0;
    //NEED_EXIT = 0;

    //if (FULLSCREEN || display_width == GAME_WIDTH)
    //{
    //    WINDOW_POS_X = 0;
    //    WINDOW_POS_Y = 0;
    //    WINDOW_WIDTH = GAME_WIDTH;
    //    WINDOW_HEIGHT = GAME_HEIGHT;
    //    dwStyle = WS_POPUP | WS_VISIBLE;
    //}
    //else
    //{
    //    WINDOW_WIDTH = GAME_WIDTH + 16;
    //    WINDOW_HEIGHT = GAME_HEIGHT + 39;
    //    dwStyle = WS_CAPTION | WS_SYSMENU | WS_GROUP;
    //    WINDOW_POS_X = (display_width - WINDOW_WIDTH) / 2;
    //    WINDOW_POS_Y = (display_height - WINDOW_HEIGHT) / 2;
    //}

    //WINDOW_HWND = CreateWindowExA(
    //    NULL, class_name, "Bedlam 2", dwStyle, WINDOW_POS_X, WINDOW_POS_Y, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hwnd, NULL);

    //ShowWindow(WINDOW_HWND, SW_SHOWNORMAL);
    //UpdateWindow(WINDOW_HWND);

    //int error_code = init_dsound();
    //if (error_code)
    //{
    //    dsound_deinit();
    //    return 6;
    //}

    //if (SURFACE_IS_LOCKED)
    //{
    //    error_code = ddraw_init(WINDOW_HWND);
    //    if (error_code)
    //    {
    //        SURFACE_IS_LOCKED = 0;
    //    }
    //    else
    //    {
    //        error_code = create_surface_palette(GAME_WIDTH, GAME_HEIGHT, 8);
    //        if (error_code)
    //        {
    //            ddraw_deinit();
    //            SURFACE_IS_LOCKED = 0;
    //        }
    //    }
    //}
    //if (!SURFACE_IS_LOCKED)
    //    return error_code;
    //clear_and_blit_screen();

    return 0;
}

// 0044DACC Bedlam 1
LRESULT WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    if (Msg == WM_DESTROY)
    {
        if (GAME_THREAD_ID != -1)
        {
            SuspendThread(HANDLE_GAME_THREAD);
        }
        activate_app(0);
        NEED_EXIT = 1;
        PostQuitMessage(0);
    }
    if (Msg == WM_MOVE && !FULLSCREEN)
    {
        WINDOW_POS_X = LOWORD(lParam);
        WINDOW_POS_Y = HIWORD(lParam);
        InvalidateRect(WINDOW_HWND, 0, 0);
    }
    if (Msg == WM_SIZE && !FULLSCREEN)
    {
        GAME_WIDTH = LOWORD(lParam);
        GAME_HEIGHT = HIWORD(lParam);
        /// dword_4EF682 = lParam;
        InvalidateRect(WINDOW_HWND, NULL, 0);
    }
    if (Msg == WM_ACTIVATEAPP)
    {
        if (!FULLSCREEN || wParam == 1) // wParam == activate
        {
            APPLICATION_ACTIVE = 1;
            activate_app(1);
            return 0;
        }
        return 1;
    }
    if (Msg == WM_SETCURSOR)
    {
        uint16_t nHittest = LOWORD(lParam);
        uint16_t wMouseMsg = HIWORD(lParam);
        if (IsIconic(hWnd) || !SURFACE_IS_LOCKED)
        { // is minimized
            nHittest = HTNOWHERE;
        }
        if (nHittest == HTCLIENT)
        { // in client area
            SetCursor(0);
            return 1;
        }
        else
        {
            HCURSOR cursor = LoadCursorA(NULL, (LPCSTR)0x7F00);
            SetCursor(cursor);
        }
        return 1;
    }
    if (Msg == WM_KEYDOWN)
    {
        if (BYTE2(static_cast<uint32_t>(lParam)) == 70)
        {
            save_screenshot();
        }
        else
        {
            keyboard(BYTE2(static_cast<uint32_t>(lParam)), 0);
        }
        return DefWindowProcA(hWnd, (UINT)Msg, wParam, lParam);
    }
    if (Msg == WM_KEYUP)
    {
        keyboard(BYTE2(static_cast<uint32_t>(lParam)), 1);
        return DefWindowProcA(hWnd, (UINT)Msg, wParam, lParam);
    }

    if (Msg == WM_LBUTTONUP)
    {
        mouse_buttons(0, 1);
        return DefWindowProcA(hWnd, (UINT)Msg, wParam, lParam);
    }
    else if (Msg == WM_LBUTTONDBLCLK)
    {
        mouse_buttons(0, 2);
        return DefWindowProcA(hWnd, (UINT)Msg, wParam, lParam);
    }
    else if (Msg == WM_RBUTTONDOWN)
    {
        mouse_buttons(1, 0);
        return DefWindowProcA(hWnd, (UINT)Msg, wParam, lParam);
    }
    else if (Msg == WM_RBUTTONUP)
    {
        mouse_buttons(1, 1);
        return DefWindowProcA(hWnd, (UINT)Msg, wParam, lParam);
    }
    else if (Msg == WM_RBUTTONDBLCLK)
    {
        mouse_buttons(1, 2);
        return DefWindowProcA(hWnd, (UINT)Msg, wParam, lParam);
    }
    if (Msg == WM_LBUTTONDOWN)
    {
        mouse_buttons(0, 0);
        return DefWindowProcA(hWnd, (UINT)Msg, wParam, lParam);
    }

    if (wParam >= 0xF100)
    {
        if (wParam > 0xF100)
        {
            if (wParam != 0xF140)
                return DefWindowProcA(hWnd, (UINT)Msg, wParam, lParam);
            return 1;
        }
        keyboard(68, 0);
    }

    return DefWindowProcA(hWnd, (UINT)Msg, wParam, lParam);
}

// 0044D93C Bedlam 1
WPARAM message_dispatcher()
{
    struct tagMSG Msg; // [esp+0h] [ebp-28h] BYREF

    memset(&Msg, 0, sizeof(Msg));

    while (!NEED_EXIT)
    {
        if (PeekMessageA(&Msg, NULL, 0, 0, PM_NOREMOVE))
        {
            if (GetMessageA(&Msg, NULL, 0, 0))
            {
                if (Msg.message != WM_ACTIVATEAPP || Msg.wParam || !FULLSCREEN)
                    DispatchMessageA(&Msg);
            }
            else
            {
                NEED_EXIT = 1;
            }
        }
    }
    return Msg.wParam;
}

// 0041C9F0 Bedlam 1
void destroy()
{
    // free_mem();
    Sleep(500u);
    send_destroy_message();
    while (1)
    {
        ;
    }
}

// 0044DA00 Bedlam 1
LRESULT send_destroy_message()
{
    return SendMessageA(WINDOW_HWND, 2u, 0, 0);
}

// 0044DE10 Bedlam 1
int error_msgbox(const char *error_str)
{
    return error_msgbox((char *)error_str);
}

// 0044DE10 Bedlam 1
int error_msgbox(char *error_str)
{
    uint8_t lock; // si
    int result;   // eax

    lock = SURFACE_IS_LOCKED;
    SURFACE_IS_LOCKED = 0;
    result = MessageBoxA(0, error_str, "Error", (UINT)16);
    SURFACE_IS_LOCKED = lock;
    return result;
}