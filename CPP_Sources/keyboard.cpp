#include "keyboard.h"

uint8_t PRESSED_KEY_ARR[257];

int32_t KEY_ESC;
int32_t KEY_F1;
int32_t KEY_F2;
int32_t KEY_F3;
int32_t KEY_1;
int32_t KEY_2;
int32_t KEY_3;
int32_t KEY_4;
int32_t KEY_5;
int32_t KEY_6;
int32_t KEY_7;
int32_t KEY_P;
int32_t KEY_M;

// 0041BE05 Bedlam 1
void keyboard(uint8_t scan_code, uint8_t key_up)
{
    if (scan_code == 72 || scan_code == 80 || scan_code == 75 || scan_code == 77)
    {
        scan_code += 128;
    }
    if (scan_code >= 0 && scan_code < 256)
        PRESSED_KEY_ARR[scan_code] = key_up == 0;
    if (PRESSED_KEY_ARR[1]) // esc
        KEY_ESC = 1;
    if (PRESSED_KEY_ARR[59]) // F1
        KEY_F1 = 1;
    if (PRESSED_KEY_ARR[60]) // F2
        KEY_F2 = 1;
    if (PRESSED_KEY_ARR[61]) // F3
        KEY_F3 = 1;
    if (PRESSED_KEY_ARR[2]) // 1
        KEY_1 = 1;
    if (PRESSED_KEY_ARR[3]) // 2
        KEY_2 = 1;
    if (PRESSED_KEY_ARR[4]) // 3
        KEY_3 = 1;
    if (PRESSED_KEY_ARR[5]) // 4
        KEY_4 = 1;
    if (PRESSED_KEY_ARR[6]) // 5
        KEY_5 = 1;
    if (PRESSED_KEY_ARR[7]) // 6
        KEY_6 = 1;
    if (PRESSED_KEY_ARR[8]) // 7
        KEY_7 = 1;
    if (PRESSED_KEY_ARR[25]) // P
        KEY_P = 1;
    if (PRESSED_KEY_ARR[50] || PRESSED_KEY_ARR[57]) // M || SPACE
        KEY_M = 1;
}
