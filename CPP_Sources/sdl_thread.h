#pragma once
#include <SDL.h>
#include <string>

class Thread
{
public:
    Thread(void *function, const std::string &name);
    ~Thread();

private:
    SDL_Thread *m_thread = nullptr;
};
