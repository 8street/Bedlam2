#include <iostream>

#include "sdl_window.h"



Window::Window(int width, int height, const std::string &title)
    : m_window_size{width, height}
{ 
    init_window(title);
}

Window::~Window()
{
    destroy();
}

int Window::init_window(const std::string &title)
{
    int ret_val = 0;
    ret_val |= SDL_Init(SDL_INIT_VIDEO);
    int window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN;
    m_window = SDL_CreateWindow(
        title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_window_size.x, m_window_size.y, window_flags);
    /*Render = SDL_CreateRenderer(
        m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);*/

   /* if (Render == NULL)
    {
        std::cout << "ERROR: created accelerated renderer is invalid. \n";
        Render = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_TARGETTEXTURE);
        if (Render == NULL)
        {
            std::cout << "ERROR: created software renderer is invalid. \n";
            ret_val |= 1;
        }
    }*/

    SDL_SetWindowMinimumSize(m_window, m_window_size.x, m_window_size.y);

   /* ret_val |= SDL_SetRenderTarget(Render, NULL);
    ret_val |= SDL_SetRenderDrawBlendMode(Render, SDL_BLENDMODE_BLEND);*/

    /*ret_val |= SDL_RenderClear(Render);
    SDL_RenderPresent(Render);*/

    /*m_screen_texture = SDL_CreateTexture(
        Render, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, m_window_size.x, m_window_size.y);
    ret_val |= SDL_SetTextureBlendMode(m_screen_texture, SDL_BLENDMODE_BLEND);*/

    if (ret_val)
    {
        std::cout << "ERROR: init window \n";
    }

    return ret_val;
}


int Window::destroy()
{
    //SDL_DestroyTexture(m_screen_texture);
    //SDL_DestroyRenderer(Render);
    SDL_DestroyWindow(m_window);
    //SDL_Quit();
    return 0;
}
