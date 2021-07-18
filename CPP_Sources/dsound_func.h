#pragma once

#include <dsound.h>
#include <stdint.h>

int init_dsound();
int load_raw_to_dsound_buffer(uint8_t* raw_ptr, int filesize, int discretization, int bit, int channels);
int duplicate_sound_buffer(int index);
extern "C" int load_raw(const char* filename);
extern "C" int load_music(const char* filename);
bool dsound_buf_is_stopped(int buffer_index);
extern "C" int play_sound(int raw_index, int x, int y, int a5);
extern "C" int play_music(int raw_index, int x, int y, int flag);
int dsound_play(int buffer_index, int pos, int samplerate, int volume, int balance, int flag);
void dsound_stop(int buffer_index);
extern "C" void stop_music();
void dsound_deinit();
int get_volume(int x, int y);
int get_balance(int x, int y);
