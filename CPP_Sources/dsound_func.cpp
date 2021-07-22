#include <cstdlib>
#include <unordered_map>
#include <vector>

#include "bedlam2.h"
#include "dsound_func.h"
#include "file.h"
#include "helper.h"
#include "sdl_event.h"

LPDIRECTSOUND ppDS;

int DSOUND_ERRORCODE;
int DSOUND_INNITED;

LPDIRECTSOUNDBUFFER PRIMARY_DSOUND_BUFFER;

std::vector<LPDIRECTSOUNDBUFFER> DSOUND_BUFFERS_ARR;
std::unordered_map<std::string, int> DSOUND_FILENAME_INDEX_MAP;

int SIMULTANEOUSLY_PLAYING_BUFFERS = 5;

std::vector<int> PLAYING_MUSIC_INDEX_ARR;

// 0044C068 Bedlam 1
int init_dsound()
{
    int ret_val = 0;
    DSBUFFERDESC lpcDSBufferDesc;
    WAVEFORMATEX waveformat;

    PRIMARY_DSOUND_BUFFER = NULL;

    DSOUND_ERRORCODE = 0;
    DSOUND_INNITED = 1;

    DSOUND_BUFFERS_ARR.clear();
    DSOUND_FILENAME_INDEX_MAP.clear();
    PLAYING_MUSIC_INDEX_ARR.clear();

    if ((uint32_t)DirectSoundCreate(NULL, &ppDS, NULL))
    {
        ret_val = 1;
        DSOUND_ERRORCODE = -1;
        DSOUND_INNITED = 0;
    }
   /* else if ((uint32_t)ppDS->SetCooperativeLevel(WINDOW_HWND, DSSCL_PRIORITY))
    {
        ret_val = 2;
        DSOUND_ERRORCODE = -1;
        DSOUND_INNITED = 0;
    }*/
    else
    {
        memset(&waveformat, 0, sizeof(waveformat));
        waveformat.wFormatTag = WAVE_FORMAT_PCM;
        waveformat.wBitsPerSample = 16;
        waveformat.nChannels = 2;
        waveformat.nSamplesPerSec = 44100;
        waveformat.nBlockAlign = (waveformat.wBitsPerSample / 8) * waveformat.nChannels;
        waveformat.nAvgBytesPerSec = waveformat.nSamplesPerSec * waveformat.nBlockAlign;

        memset(&lpcDSBufferDesc, 0, sizeof(lpcDSBufferDesc));
        lpcDSBufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
        lpcDSBufferDesc.lpwfxFormat = NULL;
        lpcDSBufferDesc.dwSize = sizeof(lpcDSBufferDesc);
        if ((uint32_t)ppDS->CreateSoundBuffer(&lpcDSBufferDesc, &PRIMARY_DSOUND_BUFFER, NULL))
        {
            ret_val = 3;
            DSOUND_ERRORCODE = -1;
            DSOUND_INNITED = 0;
        }
        // else if ((uint32_t)PRIMARY_DSOUND_BUFFER->SetFormat(&waveformat))
        //{
        //    ret_val = 4;
        //    DSOUND_ERRORCODE = -1;
        //    DSOUND_INNITED = 0;
        //}
        else
        {
            PRIMARY_DSOUND_BUFFER->Play(NULL, NULL, DSBPLAY_LOOPING);
            ret_val = 0;
        }
    }
    return ret_val;
}

// 0044C64C Bedlam 1
int load_raw_to_dsound_buffer(uint8_t *raw_ptr, int filesize, int discretization, int bit, int channels)
{
    int ret_val; // eax
    LPDIRECTSOUNDBUFFER dsound_buffer = NULL;
    DSBUFFERDESC lpcDSBufferDesc; // [esp+8h] [ebp-4Ch] OVERLAPPED BYREF
    WAVEFORMATEX waveformat;      // [esp+1Ch] [ebp-38h] BYREF
    LPVOID dest_1 = NULL;         // [esp+30h] [ebp-24h] BYREF
    LPVOID dest_2 = NULL;         // [esp+34h] [ebp-20h] BYREF
    DWORD count_1 = 0;
    DWORD count_2 = 0;

    if (DSOUND_ERRORCODE || filesize <= 0)
    {
        return -1;
    }

    memset(&waveformat, 0, sizeof(waveformat));
    waveformat.wFormatTag = WAVE_FORMAT_PCM;
    waveformat.nChannels = channels;
    waveformat.wBitsPerSample = bit;
    waveformat.nSamplesPerSec = discretization;
    waveformat.nBlockAlign = (waveformat.wBitsPerSample / 8) * waveformat.nChannels;
    waveformat.nAvgBytesPerSec = waveformat.nSamplesPerSec * waveformat.nBlockAlign;

    memset(&lpcDSBufferDesc, 0, sizeof(lpcDSBufferDesc));
    lpcDSBufferDesc.dwSize = sizeof(lpcDSBufferDesc);
    lpcDSBufferDesc.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME; // 0xE2
    lpcDSBufferDesc.lpwfxFormat = &waveformat;
    lpcDSBufferDesc.dwBufferBytes = filesize;

    if ((uint32_t)ppDS->CreateSoundBuffer(&lpcDSBufferDesc, &dsound_buffer, NULL))
    {
        return -2;
    }

    if ((uint32_t)dsound_buffer->Lock(0, filesize, &dest_1, &count_1, &dest_2, &count_2, 0))
    {
        dsound_buffer->Release();
        dsound_buffer = NULL;
        return -2;
    }
    memcpy(dest_1, raw_ptr, count_1);
    if (dest_2)
    {
        memcpy(dest_2, &raw_ptr[count_1], count_2);
    }

    if ((uint32_t)dsound_buffer->Unlock(dest_1, count_1, dest_2, count_2))
    {
        dsound_buffer->Release();
        dsound_buffer = NULL;
        ret_val = -2;
    }
    else
    {
        DSOUND_BUFFERS_ARR.push_back(dsound_buffer);
        ret_val = static_cast<int>(DSOUND_BUFFERS_ARR.size()) - 1;
    }
    return ret_val;
}

// 0044C828 Bedlam 1
int duplicate_sound_buffer(int index)
{
    LPDIRECTSOUNDBUFFER dsound_buffer = NULL;

    if (DSOUND_ERRORCODE || index < 0)
    {
        return -1;
    }

    ppDS->DuplicateSoundBuffer(DSOUND_BUFFERS_ARR[index], &dsound_buffer);
    DSOUND_BUFFERS_ARR.push_back(dsound_buffer);
    return static_cast<int>(DSOUND_BUFFERS_ARR.size()) - 1;
}

// 0043A39C Bedlam 1
int load_raw(const char *filename)
{
    int num_copy = 0; // edx
    int raw_index;    // ebx

    std::string filename_string(filename);

    auto search = DSOUND_FILENAME_INDEX_MAP.find(filename_string);
    if (search != DSOUND_FILENAME_INDEX_MAP.end())
    {
        return search->second;
    }

    File raw_file(filename_string);

    raw_index = load_raw_to_dsound_buffer(raw_file.get_ptr(), static_cast<int>(raw_file.get_size()), 11025, 8, 1);
    DSOUND_FILENAME_INDEX_MAP.emplace(filename_string, raw_index);
    while (num_copy < SIMULTANEOUSLY_PLAYING_BUFFERS)
    {
        duplicate_sound_buffer(raw_index);
        num_copy++;
        SDL_events();
    }
    return raw_index;
}

int load_music(const char *filename)
{
    std::string filename_string(filename);

    auto search = DSOUND_FILENAME_INDEX_MAP.find(filename_string);
    if (search != DSOUND_FILENAME_INDEX_MAP.end())
    {
        return search->second;
    }

    File raw_file(filename_string);

    const int raw_index = load_raw_to_dsound_buffer(raw_file.get_ptr(), static_cast<int>(raw_file.get_size()), 11025, 8, 1);
    DSOUND_FILENAME_INDEX_MAP.emplace(filename_string, raw_index);
    return raw_index;
}

// 0044C5AC Bedlam 1
bool dsound_buf_is_stopped(int buffer_index)
{
    bool is_stopped = true;
    DWORD status = NULL;

    if (DSOUND_ERRORCODE)
    {
        return true;
    }

    if (buffer_index >= 0 && buffer_index < static_cast<int>(DSOUND_BUFFERS_ARR.size()) && DSOUND_BUFFERS_ARR[buffer_index])
    {
        DSOUND_BUFFERS_ARR[buffer_index]->GetStatus(&status);
        if (status == DSBSTATUS_PLAYING)
        {
            is_stopped = false;
        }
    }
    return is_stopped;
}

// 0043A48E Bedlam 1
int play_sound(int raw_index, int x, int y, int a5)
{
    //int balance; // [esp+0h] [ebp-18h]
    //int volume;  // [esp+4h] [ebp-14h]

    //if (x == -1 && y == -1)
    //{
    //    volume = 32768;
    //    balance = 0;
    //}
    //else
    //{
    //    volume = get_volume(x, y);
    //    balance = get_balance(x, y);
    //}

    //int first_stopped_index = 0; // ebp
    //int i = 0;
    //for (int raw2 = raw_index; raw2 < raw_index + SIMULTANEOUSLY_PLAYING_BUFFERS; raw2++)
    //{
    //    if (dsound_buf_is_stopped(raw2))
    //    {
    //        first_stopped_index = i;
    //        break;
    //    }
    //    i++;
    //}

    //int play_index = raw_index + first_stopped_index;

    //if (!first_stopped_index)
    //{
    //    dsound_stop(play_index);
    //}
    //return dsound_play(play_index, 0, 11025, volume, balance, 0);
    return 0;
}

int play_music(int raw_index, int x, int y, int flag)
{
    int volume = 32768;
    int balance = 0;

    int first_stopped_index = 0; // ebp
    int i = 0;

    if (!dsound_buf_is_stopped(raw_index))
    {
        dsound_stop(raw_index);
    }
    PLAYING_MUSIC_INDEX_ARR.push_back(raw_index);
    return dsound_play(raw_index, 0, 11025, volume, balance, DSBPLAY_LOOPING);
}

// 0044C4A8 Bedlam 1
int dsound_play(int buffer_index, int pos, int samplerate, int volume, int balance, int flag)
{
    if (DSOUND_ERRORCODE)
    {
        return -1;
    }
    if (buffer_index >= 0 && buffer_index < static_cast<int>(DSOUND_BUFFERS_ARR.size()))
    {
        if (DSOUND_BUFFERS_ARR[buffer_index])
        {
            if (volume)
            {
                if (dsound_buf_is_stopped(buffer_index))
                {
                    long vol = (volume - 32768) * 10000 / 32768;
                    long bal = balance * 10000 / 32768;
                    DSOUND_BUFFERS_ARR[buffer_index]->SetCurrentPosition(pos);
                    DSOUND_BUFFERS_ARR[buffer_index]->SetFrequency(samplerate);
                    DSOUND_BUFFERS_ARR[buffer_index]->SetVolume(vol);
                    DSOUND_BUFFERS_ARR[buffer_index]->SetPan(bal);
                    DSOUND_BUFFERS_ARR[buffer_index]->Play(0, 0, flag);
                }
            }
        }
    }
    return buffer_index;
}

// 0044C904 Bedlam 1
void dsound_stop(int buffer_index)
{
    if (!DSOUND_ERRORCODE && buffer_index >= 0 && buffer_index < static_cast<int>(DSOUND_BUFFERS_ARR.size()))
    {
        if (DSOUND_BUFFERS_ARR[buffer_index])
        {
            DSOUND_BUFFERS_ARR[buffer_index]->Stop();
        }
    }
}

void stop_music()
{
    for (int index : PLAYING_MUSIC_INDEX_ARR)
    {
        if (!DSOUND_ERRORCODE && index >= 0 && index < static_cast<int>(DSOUND_BUFFERS_ARR.size()))
        {
            if (DSOUND_BUFFERS_ARR[index])
            {
                DSOUND_BUFFERS_ARR[index]->Stop();
            }
        }
    }
    PLAYING_MUSIC_INDEX_ARR.clear();
}

// 0044C20C Bedlam 1
void dsound_deinit()
{
    for (auto dsound_buffer : DSOUND_BUFFERS_ARR)
    {
        dsound_buffer->Stop();
        dsound_buffer->Release();
        dsound_buffer = NULL;
    }
    DSOUND_BUFFERS_ARR.clear();
    DSOUND_FILENAME_INDEX_MAP.clear();
    PLAYING_MUSIC_INDEX_ARR.clear();
    if (PRIMARY_DSOUND_BUFFER)
    {
        PRIMARY_DSOUND_BUFFER->Stop();
        PRIMARY_DSOUND_BUFFER->Release();
        PRIMARY_DSOUND_BUFFER = NULL;
    }
    if (ppDS)
    {
        ppDS->Release();
        ppDS = NULL;
    }
}

// 0043A3E0 Bedlam 1
int get_volume(int x, int y)
{
    int volume = 32768 - 8 * get_distance(y - screen_y_pos - (x - screen_x_pos), -((x - screen_x_pos + y - screen_y_pos) >> 1));
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

// 0043A447 Bedlam 1
int get_balance(int x, int y)
{
    int balance = -128 * (y - screen_y_pos - (x - screen_x_pos));
    if (balance < -32768)
    {
        balance = -32768;
    }
    else if (balance > 32768)
    {
        balance = 32768;
    }
    return balance;
}