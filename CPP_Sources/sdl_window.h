#pragma once
#include <SDL.h>
#include <string>

class Window
{
public:
    Window(int width = 640, int height = 480, const std::string &title = "Window");
    ~Window();
    int destroy();

private:
    int init_window(const std::string &title);
    SDL_Window *m_window = nullptr;
    SDL_Point m_window_size = {0, 0};
    SDL_Texture *m_screen_texture = nullptr;
};