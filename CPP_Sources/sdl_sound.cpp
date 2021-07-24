#include <iostream>
#include <SDL.h>

#include "bedlam2.h"
#include "helper.h"
#include "sdl_sound.h"

//#define MIX_EFFECTSMAXSPEED "MIX_EFFECTSMAXSPEED"
#define MIX_MAX_BALANCE 255

Sound SOUND_SYSTEM;

Sound::Sound()
{
}

Sound::~Sound()
{
    Mix_CloseAudio();
    SDL_CloseAudio();
}

int Sound::init()
{
    int ret_val = 0;
    ret_val |= SDL_Init(SDL_INIT_AUDIO);

    if (ret_val)
    {
        std::cout << "ERROR: SDL init audio \n";
    }

    ret_val |= Mix_OpenAudio(11025, AUDIO_U8, 2, 16);

    if (ret_val)
    {
        std::cout << "ERROR: Mix_OpenAudio \n";
    }

    m_num_simultaneously_playing_channels = 10;
    // Allocate check
    int max_channels = 104 * m_num_simultaneously_playing_channels;
    int num_channels = Mix_AllocateChannels(max_channels);
    int num_reserve_channels = Mix_ReserveChannels(max_channels);
    if (num_channels != max_channels || num_reserve_channels != max_channels)
    {
        std::cout << "ERROR: allocate channels. Current channels number is " << num_channels << "\n";
        ret_val |= -1;
    }

    ret_val |= Mix_Volume(-1, MIX_MAX_VOLUME);
    
    return ret_val;
}

int Sound::add_raw(const std::string &path)
{
    // if we already have this file, just return file index
    if (get_raw_index(path) >= 0)
    {
        return get_raw_index(path);
    }

    m_raws.push_back(RAW_File(path));
    const int raw_index = get_last_raw_index();
    m_filename_index_map.emplace(path, raw_index);
    Mix_VolumeChunk(m_raws[raw_index].get_chunk(), MIX_MAX_VOLUME);
    Mix_AllocateChannels(m_num_simultaneously_playing_channels * (raw_index + 1));
    return raw_index;
}

int Sound::play_raw(int index, int x, int y, bool loop)
{
    int ret_val = 0;
    int balance = 0;
    int l_balance = 0;
    int r_balance = 0;
    int volume = 0;
    int palying_times = 0;

    if (x == -1 && y == -1)
    {
        volume = MIX_MAX_VOLUME;
        l_balance = MIX_MAX_BALANCE;
        r_balance = MIX_MAX_BALANCE;
    }
    else
    {
        volume = get_volume(x, y) >> 8; // max 128
        balance = get_balance(x, y) >> 7; // max 255
        if (balance > 0)
        {
            l_balance = balance - MIX_MAX_BALANCE;
            r_balance = MIX_MAX_BALANCE;
        }
        else
        {
            l_balance = MIX_MAX_BALANCE;
            r_balance = balance + MIX_MAX_BALANCE;
        }
    }

    int free_channel_index = get_first_free_channel(index);

    ret_val |= Mix_Volume(free_channel_index, volume);
    ret_val |= Mix_SetPanning(free_channel_index, (uint8_t)l_balance, (uint8_t)r_balance);
    if (loop)
    {
        palying_times = -1;
    }
    ret_val |= Mix_PlayChannel(free_channel_index, m_raws[index].get_chunk(), palying_times);
    return ret_val;
}

int Sound::stop()
{
    return Mix_HaltChannel(-1);
}
int Sound::stop(int index)
{
    return Mix_HaltChannel(index);
}

int Sound::fade_stop(int ms)
{
    return Mix_FadeOutChannel(-1, ms);
}

int Sound::fade_stop(int index, int ms)
{
    return Mix_FadeOutChannel(index, ms);
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

int Sound::get_first_free_channel(int index) const
{
    for (int i = index; i < index + m_num_simultaneously_playing_channels; i++)
    {
        if (!Mix_Playing(i))
        {
            return i;
        }
    }
    Mix_HaltChannel(index);
    return index;
}

int Sound::get_volume(int x, int y) const
{
    int volume = 32768 - 32 * get_distance(y - screen_y_pos - (x - screen_x_pos), -((x - screen_x_pos + y - screen_y_pos) >> 1));
    if (volume < 0)
    {
        volume = 0;
    }
    else if (volume > 32768)
    {
        volume = 32768;
    }
    return volume;
}
int Sound::get_balance(int x, int y) const
{
    int balance = -224 * (y - screen_y_pos - (x - screen_x_pos));
    if (balance < -32640)
    {
        balance = -32640;
    }
    else if (balance > 32640)
    {
        balance = 32640;
    }
    return balance;
}

//////// Function calls from bedlam2.asm ///////////

int load_raw(const char *filename)
{
    return SOUND_SYSTEM.add_raw(filename);
}

int load_music(const char *filename)
{
    return SOUND_SYSTEM.add_raw(filename);
}

int play_sound(int raw_index, int x, int y, int a5)
{
    return SOUND_SYSTEM.play_raw(raw_index, x, y);
}

int play_music(int raw_index, int x, int y, int flag)
{
    return SOUND_SYSTEM.play_raw(raw_index, x, y, true);
}

void stop_music()
{
    SOUND_SYSTEM.fade_stop(10);
}