
#include "bedlam2.h"
#include "main.h"
#include "mouse.h"
#include "sdl_draw.h"
#include "sdl_timer.h"
#include "sdl_sound.h"

int main(int argc, char *argv[])
{
    init_video();
    //init_dsound();
    init_timer();
    WINDOW_CURSOR.init();

    Sound game_sound;
    game_sound.init();
    int shoop = game_sound.add_raw("SOUND/LOOPS/SHOP2.RAW");
    game_sound.play_raw(shoop);

    return main_(0, NULL, NULL);
}

void free_win()
{
    //stop_wav_music();
    //timer_deinit();
    //ddraw_deinit();
    //dsound_deinit();
}
