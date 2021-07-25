#include <iostream>

#include "bedlam2.h"
#include "main.h"
#include "mouse.h"
#include "sdl_timer.h"
#include "sdl_sound.h"
#include "sdl_window.h"

int main(int argc, char *argv[])
{
#ifdef _DEBUG
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    //atexit([] { system("PAUSE"); });
#endif
    GAME_WINDOW.init();
    WINDOW_CURSOR.init();
    GAME_TIMER.init(10);
    SOUND_SYSTEM.init();

    // Run bedlam 2 main function
    return main_(0, NULL, NULL);
}

void free_win()
{
    // Place all destroy and deinits here

}
