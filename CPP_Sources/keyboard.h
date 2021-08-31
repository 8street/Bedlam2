#pragma once
#include <SDL.h>
#include <stdint.h>
#include <unordered_map>

// exports to bedlam2.asm
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
// end exports

// Bedlam 2 ordinary scancodes
enum class Scancode
{
    ESC = 1,
    key_1,
    key_2,
    key_3,
    key_4,
    key_5,
    key_6,
    key_7,
    key_8,
    key_9,
    key_0,
    Minus,
    Equals,
    BackSpace,
    Tab,
    Q,
    W,
    E,
    R,
    T,
    Y,
    U,
    I,
    O,
    P,
    Leftbracket,  // [
    Rightbracket, // ]
    Enter,
    CTRL,
    A,
    S,
    D,
    F,
    G,
    H,
    J,
    K,
    L,
    Semicolon,   // ;
    Apostrophe1, // '
    Apostrophe2, // `
    LShift,
    Backslash,
    Z,
    X,
    C,
    V,
    B,
    N,
    M,
    Comma,
    Point,
    Slash,
    RShift,
    PrtSc,
    Alt,
    Space,
    Caps,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    Num,
    Scroll,
    Num_Home_7,
    Num_Up_8,
    Num_PgUp_9,
    Num_minus,
    Num_Left_4,
    Num_Center_5,
    Num_Right_6,
    Num_Plus,
    Num_End_1,
    Num_Down_2,
    Num_PgDn_3,
    Num_Ins,
    Num_Del,
};

class Keyboard
{
public:
    Keyboard();
    ~Keyboard();
    int key_up(SDL_Keycode sdl_keycode);
    int key_down(SDL_Keycode sdl_keycode);

private:
    void bedlam_keystates_update(uint8_t scan_code, uint8_t key_up);
    uint8_t get_ordinary_scancode_from_sdl_keycode(SDL_Keycode sdl_keycode);
    std::unordered_map<SDL_Keycode, Scancode> m_keycode_scancode_map;
};

extern Keyboard GAME_KEYBOARD;