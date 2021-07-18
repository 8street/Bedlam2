#pragma once
#include <stdint.h>

extern "C" uint8_t PRESSED_KEY_ARR[257];

extern "C" int32_t KEY_ESC;
extern "C" int32_t KEY_F1;
extern "C" int32_t KEY_F2;
extern "C" int32_t KEY_F3;
extern "C" int32_t KEY_1;
extern "C" int32_t KEY_2;
extern "C" int32_t KEY_3;
extern "C" int32_t KEY_4;
extern "C" int32_t KEY_5;
extern "C" int32_t KEY_6;
extern "C" int32_t KEY_7;
extern "C" int32_t KEY_P;
extern "C" int32_t KEY_M;


void keyboard(uint8_t scan_code, uint8_t key_up);
