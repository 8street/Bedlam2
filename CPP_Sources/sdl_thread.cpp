#include "sdl_thread.h"

Thread::Thread(void *function, const std::string &name)
{
    m_thread = SDL_CreateThread((SDL_ThreadFunction)function, name.c_str(), (void *)NULL);
}

Thread::~Thread()
{

}