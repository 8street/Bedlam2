#pragma once

#include <stdint.h>

#include "sdl_cursor.h"

extern cursor WINDOW_CURSOR;

extern int16_t CURSOR_X1;
extern int16_t CURSOR_Y1;

extern "C" volatile int CURSOR_ICON;
extern int32_t CURSOR_ICON2;

extern uint16_t CURSOR_SURFACE_SIZE;

extern "C" volatile uint8_t UPDATE_CURSOR_BY_TIMER;
extern uint8_t CURSOR_BY_TIMER;

extern uint32_t CURSOR_UNKNOWN;
extern uint8_t CURSOR_IS_BLITTING;

extern int32_t MOUSE_BUTTONS_STATE;
extern "C" volatile int32_t MOUSE_BUTTONS_STATE1;

extern "C" volatile int32_t CURSOR_POS_X;
extern "C" volatile int32_t CURSOR_POS_Y;

extern "C" volatile int32_t CURSOR_POS_LCLICK_X;
extern "C" volatile int32_t CURSOR_POS_LCLICK_Y;
extern "C" volatile int32_t CURSOR_POS_RCLICK_X;
extern "C" volatile int32_t CURSOR_POS_RCLICK_Y;

extern "C" volatile int32_t MOUSE_CLICK;

enum CURSOR_ICON
{
    ICON_CROSSHAIR = 0,
    ICON_CURSOR = 93,
    ICON_WAIT = 144,
};

void mouse_update();
extern "C" int hide_cursor();
void get_cursor_pos(int *x, int *y);
extern "C" void show_cursor();
void mouse_buttons(uint16_t r_butt, uint16_t l_button);
