#include "keyboard.h"

// exports to bedlam2.asm
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
// end exports

Keyboard GAME_KEYBOARD;

Keyboard::Keyboard()
{
    m_keycode_scancode_map.emplace(SDLK_ESCAPE, Scancode::ESC);
    m_keycode_scancode_map.emplace(SDLK_1, Scancode::key_1);
    m_keycode_scancode_map.emplace(SDLK_2, Scancode::key_2);
    m_keycode_scancode_map.emplace(SDLK_3, Scancode::key_3);
    m_keycode_scancode_map.emplace(SDLK_4, Scancode::key_4);
    m_keycode_scancode_map.emplace(SDLK_5, Scancode::key_5);
    m_keycode_scancode_map.emplace(SDLK_6, Scancode::key_6);
    m_keycode_scancode_map.emplace(SDLK_7, Scancode::key_7);
    m_keycode_scancode_map.emplace(SDLK_8, Scancode::key_8);
    m_keycode_scancode_map.emplace(SDLK_9, Scancode::key_9);
    m_keycode_scancode_map.emplace(SDLK_0, Scancode::key_0);
    m_keycode_scancode_map.emplace(SDLK_MINUS, Scancode::Minus);
    m_keycode_scancode_map.emplace(SDLK_EQUALS, Scancode::Equals);
    m_keycode_scancode_map.emplace(SDLK_BACKSPACE, Scancode::BackSpace);
    m_keycode_scancode_map.emplace(SDLK_TAB, Scancode::Tab);
    m_keycode_scancode_map.emplace(SDLK_q, Scancode::Q);
    m_keycode_scancode_map.emplace(SDLK_w, Scancode::W);
    m_keycode_scancode_map.emplace(SDLK_e, Scancode::E);
    m_keycode_scancode_map.emplace(SDLK_r, Scancode::R);
    m_keycode_scancode_map.emplace(SDLK_t, Scancode::T);
    m_keycode_scancode_map.emplace(SDLK_y, Scancode::Y);
    m_keycode_scancode_map.emplace(SDLK_u, Scancode::U);
    m_keycode_scancode_map.emplace(SDLK_i, Scancode::I);
    m_keycode_scancode_map.emplace(SDLK_o, Scancode::O);
    m_keycode_scancode_map.emplace(SDLK_p, Scancode::P);
    m_keycode_scancode_map.emplace(SDLK_LEFTBRACKET, Scancode::Leftbracket);
    m_keycode_scancode_map.emplace(SDLK_RIGHTBRACKET, Scancode::Rightbracket);
    m_keycode_scancode_map.emplace(SDLK_RETURN, Scancode::Enter);
    m_keycode_scancode_map.emplace(SDLK_KP_ENTER, Scancode::Enter);
    m_keycode_scancode_map.emplace(SDLK_LCTRL, Scancode::CTRL);
    m_keycode_scancode_map.emplace(SDLK_a, Scancode::A);
    m_keycode_scancode_map.emplace(SDLK_s, Scancode::S);
    m_keycode_scancode_map.emplace(SDLK_d, Scancode::D);
    m_keycode_scancode_map.emplace(SDLK_f, Scancode::F);
    m_keycode_scancode_map.emplace(SDLK_g, Scancode::G);
    m_keycode_scancode_map.emplace(SDLK_h, Scancode::H);
    m_keycode_scancode_map.emplace(SDLK_j, Scancode::J);
    m_keycode_scancode_map.emplace(SDLK_k, Scancode::K);
    m_keycode_scancode_map.emplace(SDLK_l, Scancode::L);
    m_keycode_scancode_map.emplace(SDLK_COLON, Scancode::Semicolon);
    m_keycode_scancode_map.emplace(SDLK_SEMICOLON, Scancode::Semicolon);
    m_keycode_scancode_map.emplace(SDLK_BACKQUOTE, Scancode::Apostrophe1);
    m_keycode_scancode_map.emplace(SDLK_BACKQUOTE, Scancode::Apostrophe2);
    m_keycode_scancode_map.emplace(SDLK_LSHIFT, Scancode::LShift);
    m_keycode_scancode_map.emplace(SDLK_BACKSLASH, Scancode::Backslash);
    m_keycode_scancode_map.emplace(SDLK_z, Scancode::Z);
    m_keycode_scancode_map.emplace(SDLK_x, Scancode::X);
    m_keycode_scancode_map.emplace(SDLK_c, Scancode::C);
    m_keycode_scancode_map.emplace(SDLK_v, Scancode::V);
    m_keycode_scancode_map.emplace(SDLK_b, Scancode::B);
    m_keycode_scancode_map.emplace(SDLK_n, Scancode::N);
    m_keycode_scancode_map.emplace(SDLK_m, Scancode::M);
    m_keycode_scancode_map.emplace(SDLK_COMMA, Scancode::Comma);
    m_keycode_scancode_map.emplace(SDLK_PERIOD, Scancode::Point);
    m_keycode_scancode_map.emplace(SDLK_SLASH, Scancode::Slash);
    m_keycode_scancode_map.emplace(SDLK_RSHIFT, Scancode::RShift);
    m_keycode_scancode_map.emplace(SDLK_PRINTSCREEN, Scancode::PrtSc);
    m_keycode_scancode_map.emplace(SDLK_LALT, Scancode::Alt);
    m_keycode_scancode_map.emplace(SDLK_RALT, Scancode::Alt);
    m_keycode_scancode_map.emplace(SDLK_SPACE, Scancode::Space);
    m_keycode_scancode_map.emplace(SDLK_CAPSLOCK, Scancode::Caps);
    m_keycode_scancode_map.emplace(SDLK_F1, Scancode::F1);
    m_keycode_scancode_map.emplace(SDLK_F2, Scancode::F2);
    m_keycode_scancode_map.emplace(SDLK_F3, Scancode::F3);
    m_keycode_scancode_map.emplace(SDLK_F4, Scancode::F4);
    m_keycode_scancode_map.emplace(SDLK_F5, Scancode::F5);
    m_keycode_scancode_map.emplace(SDLK_F6, Scancode::F6);
    m_keycode_scancode_map.emplace(SDLK_F7, Scancode::F7);
    m_keycode_scancode_map.emplace(SDLK_F8, Scancode::F8);
    m_keycode_scancode_map.emplace(SDLK_F9, Scancode::F9);
    m_keycode_scancode_map.emplace(SDLK_F10, Scancode::F10);
    m_keycode_scancode_map.emplace(SDLK_NUMLOCKCLEAR, Scancode::Num);
    m_keycode_scancode_map.emplace(SDLK_SCROLLLOCK, Scancode::Scroll);
    m_keycode_scancode_map.emplace(SDLK_KP_7, Scancode::Num_Home_7);
    m_keycode_scancode_map.emplace(SDLK_KP_8, Scancode::Num_Up_8);
    m_keycode_scancode_map.emplace(SDLK_KP_9, Scancode::Num_PgUp_9);
    m_keycode_scancode_map.emplace(SDLK_KP_MINUS, Scancode::Num_minus);
    m_keycode_scancode_map.emplace(SDLK_KP_4, Scancode::Num_Left_4);
    m_keycode_scancode_map.emplace(SDLK_KP_5, Scancode::Num_Center_5);
    m_keycode_scancode_map.emplace(SDLK_KP_6, Scancode::Num_Right_6);
    m_keycode_scancode_map.emplace(SDLK_KP_PLUS, Scancode::Num_Plus);
    m_keycode_scancode_map.emplace(SDLK_KP_1, Scancode::Num_End_1);
    m_keycode_scancode_map.emplace(SDLK_KP_2, Scancode::Num_Down_2);
    m_keycode_scancode_map.emplace(SDLK_KP_3, Scancode::Num_PgDn_3);
    m_keycode_scancode_map.emplace(SDLK_KP_0, Scancode::Num_Ins);
    m_keycode_scancode_map.emplace(SDLK_KP_PERIOD, Scancode::Num_Del);
}

Keyboard::~Keyboard()
{
}

int Keyboard::key_up(SDL_Keycode sdl_keycode)
{
    uint8_t bedlam_scancode = get_ordinary_scancode_from_sdl_keycode(sdl_keycode);
    bedlam_keystates_update(bedlam_scancode, true);
    return 0;
}

int Keyboard::key_down(SDL_Keycode sdl_keycode)
{
    uint8_t bedlam_scancode = get_ordinary_scancode_from_sdl_keycode(sdl_keycode);
    bedlam_keystates_update(bedlam_scancode, false);
    return 0;
}

// 0041BE05 Bedlam 1
void Keyboard::bedlam_keystates_update(uint8_t scan_code, uint8_t key_up)
{
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

uint8_t Keyboard::get_ordinary_scancode_from_sdl_keycode(SDL_Keycode sdl_keycode)
{
    auto search = m_keycode_scancode_map.find(sdl_keycode);
    if (search != m_keycode_scancode_map.end())
    {
        return static_cast<uint8_t>(search->second);
    }
    return 0;
}