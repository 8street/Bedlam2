#include <iostream>
#include <SDL.h>

#include "sdl_sound.h"


Sound::Sound()
{
}

Sound::~Sound()
{
    //SDL_CloseAudio();
    //Mix_CloseAudio();
}

int Sound::init()
{
    int ret_val = 0;
    ret_val |= SDL_Init(SDL_INIT_AUDIO);
    if (ret_val)
    {
        std::cout << "ERROR: SDL init audio \n";
    }
    ret_val |= Mix_OpenAudio(11025, AUDIO_U8, 2, 2048);
    if (ret_val)
    {
        std::cout << "ERROR: Mix_OpenAudio \n";
    }
    

    int num_channels = Mix_AllocateChannels(256);
    if (num_channels != 256)
    {
        std::cout << "ERROR: allocate channels. Current channels number is " << num_channels << "\n";
        ret_val |= -1;
    }

    ret_val |= Mix_Volume(-1, MIX_MAX_VOLUME);

    return ret_val;
}

int Sound::add_raw(const std::string& path)
{
    // if we already have this file, just return file index
    if (get_raw_index(path) >= 0)
    {
        return get_raw_index(path);
    }

    m_raws.push_back(RAW_File(path));
    m_filename_index_map.emplace(path, get_last_raw_index());
    return get_last_raw_index();
}

int Sound::play_raw(int index) const
{
    const Mix_Chunk *current_chunk = m_raws[index].get_chunk();
    Mix_Chunk *current_chunk1 = (Mix_Chunk *)current_chunk;
    Mix_VolumeChunk(current_chunk1, MIX_MAX_VOLUME);
    int i = Mix_PlayChannel(-1, current_chunk1, 0);
    return i;
}

int Sound::stop()
{
    return Mix_HaltChannel(-1);
}

int Sound::get_raw_index(const std::string &path) const
{
    auto search = m_filename_index_map.find(path);
    if (search != m_filename_index_map.end())
    {
        return search->second;
    }
    return -1;
}

int Sound::get_last_raw_index() const
{
    return static_cast<int>(m_raws.size()) - 1;
}