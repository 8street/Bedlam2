
#include "bedlam2.h"
#include "main.h"
#include "mouse.h"
#include "sdl_draw.h"
#include "sdl_timer.h"
#include "sdl_sound.h"

int main(int argc, char *argv[])
{
    init_video();
    init_timer();
    WINDOW_CURSOR.init();
    SOUND_SYSTEM.init();

    return main_(0, NULL, NULL);
}

void free_win()
{
    //timer_deinit();
    //ddraw_deinit();
    //dsound_deinit();
}
