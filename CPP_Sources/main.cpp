#include <process.h>
#include <stdio.h>

#include "bedlam2.h"
#include "dsound_func.h"
#include "ingame_music.h"
#include "main.h"
#include "mouse.h"
#include "sdl_draw.h"
#include "sdl_event.h"
#include "sdl_thread.h"
#include "sdl_timer.h"

int main(int argc, char *argv[])
{
    init_video();
    init_dsound();
    init_timer();
    WINDOW_CURSOR.init();


    return main_(0, NULL, NULL);
}

void free_win()
{
    stop_wav_music();
    //timer_deinit();
    //ddraw_deinit();
    dsound_deinit();
}
