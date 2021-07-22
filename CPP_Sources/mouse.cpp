#include <SDL.h>

#include "mouse.h"
#include "bedlam2.h"
#include "sdl_draw.h"
#include "helper.h"

cursor WINDOW_CURSOR;

int16_t CURSOR_X1;
int16_t CURSOR_Y1;

volatile int CURSOR_ICON;

volatile uint8_t UPDATE_CURSOR_BY_TIMER;

volatile int32_t CURSOR_POS_X;
volatile int32_t CURSOR_POS_Y;

volatile int32_t CURSOR_POS_LCLICK_X;
volatile int32_t CURSOR_POS_LCLICK_Y;
volatile int32_t CURSOR_POS_RCLICK_X;
volatile int32_t CURSOR_POS_RCLICK_Y;

int32_t MOUSE_BUTTONS_STATE;
volatile int32_t MOUSE_BUTTONS_STATE1;

volatile int32_t MOUSE_CLICK;

int32_t MOUSE_UP;

// 00425AB9 Bedlam 1
void mouse_update()
{
    int pos_x; // [esp+0h] [ebp-1Ch] BYREF
    int pos_y; // [esp+4h] [ebp-18h] OVERLAPPED BYREF
    uint32_t prev_state;

    prev_state = MOUSE_CLICK;
    if (!MOUSE_CLICK)
    {
        if ((MOUSE_BUTTONS_STATE1 & 3) != 0)
        {
            if (MOUSE_UP)
            {
                MOUSE_CLICK = 1;
                MOUSE_UP = prev_state;
            }
        }
        else
        {
            MOUSE_UP = 1;
        }
    }
    MOUSE_BUTTONS_STATE1 = MOUSE_BUTTONS_STATE;
    get_cursor_pos(&pos_x, &pos_y);
    CURSOR_POS_X = pos_x;
    CURSOR_POS_Y = pos_y;
    if ((MOUSE_BUTTONS_STATE1 & 1) != 0)
    {
        CURSOR_POS_LCLICK_X = CURSOR_POS_X;
        CURSOR_POS_LCLICK_Y = CURSOR_POS_Y;
    }
    if ((MOUSE_BUTTONS_STATE1 & 2) != 0)
    {
        CURSOR_POS_RCLICK_X = CURSOR_POS_X;
        CURSOR_POS_RCLICK_Y = CURSOR_POS_Y;
    }

    if (game_is_playing)
    {
        if (CURSOR_POS_X < 480)
        {
            CURSOR_ICON = ICON_CROSSHAIR;
        }
        else
        {
            CURSOR_ICON = ICON_CURSOR;
        }
    }
}

// 0042391D Bedlam 1
int hide_cursor()
{
    return WINDOW_CURSOR.hide_cursor();
}

// 0044B428 Bedlam 1
void get_cursor_pos(int *x, int *y)
{
    SDL_GetMouseState(x, y);
}

// 0042392D Bedlam 1
void show_cursor()
{
    WINDOW_CURSOR.show_cursor();
}

// 0041BF35 Bedlam 1
void mouse_buttons(uint16_t r_butt, uint16_t l_button)
{
    if (!r_butt && !l_button)
    {
        MOUSE_BUTTONS_STATE |= 1;
    }
    if (!r_butt && l_button == 1)
    {
        MOUSE_BUTTONS_STATE &= 0xFE;
    }
    if (r_butt == 1 && !l_button)
    {
        MOUSE_BUTTONS_STATE |= 2u;
    }
    if (r_butt == 1 && l_button == 1)
    {
        MOUSE_BUTTONS_STATE &= 0xFFFFFFFD;
    }
}
