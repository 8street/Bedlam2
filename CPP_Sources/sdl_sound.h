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
    int play_raw(int index) const;
    int stop();

private:
    int get_raw_index(const std::string &path) const;
    int get_last_raw_index() const;
    std::vector<RAW_File> m_raws;
    std::unordered_map<std::string, int> m_filename_index_map;
};
