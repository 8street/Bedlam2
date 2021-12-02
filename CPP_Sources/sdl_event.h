#pragma once
#include <SDL.h>

extern "C" int SDL_events();

int quit_events(const SDL_Event &event);
int mouse_events(const SDL_Event &event);
int palette_events(const SDL_Event &event);
int cursor_events(const SDL_Event &event);
int keyboard_events(const SDL_Event &event);
int window_events(const SDL_Event &event);