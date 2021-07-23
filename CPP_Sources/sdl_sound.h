#pragma once
#include <SDL_mixer.h>
#include <vector>
#include <unordered_map>

#include "raw_file.h"

class Sound
{
public:
    Sound();
    ~Sound();
    int init();
    int add_raw(const std::string &path);
    int play_raw(int index, int x = -1, int y = -1, bool loop = false);
    int stop();
    int stop(int index);
    int fade_stop(int ms);
    int fade_stop(int index, int ms);

private:
    int get_raw_index(const std::string &path) const;
    int get_last_raw_index() const;
    int get_first_free_channel(int index) const;
    int get_volume(int x, int y) const;
    int get_balance(int x, int y) const;
    std::vector<RAW_File> m_raws;
    std::unordered_map<std::string, int> m_filename_index_map;
    int m_num_simultaneously_playing_channels = 0;
};

extern Sound SOUND_SYSTEM;

//////// Function calls from bedlam2.asm ///////////

extern "C" int load_raw(const char *filename);
extern "C" int load_music(const char *filename);
extern "C" int play_sound(int raw_index, int x, int y, int a5);
extern "C" int play_music(int raw_index, int x, int y, int flag);
extern "C" void stop_music();
