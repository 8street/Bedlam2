#pragma once
#include <string>

#include <SDL_mixer.h>

#include "wav.h"

class Sound_chunk
{
public:
    Sound_chunk();
    Sound_chunk(const std::string &path);
    Sound_chunk(const WAV &wav_file);
    Sound_chunk(const Sound_chunk &chunk);
    ~Sound_chunk();

    Sound_chunk &operator=(const Sound_chunk &chunk) = delete;

    int load_chunk(const WAV &wav_file);
    Mix_Chunk *get_chunk();
    const Mix_Chunk *get_chunk() const;
    bool is_allocated() const;
    uint32_t get_buf_size() const;

private:
    int destroy_chunk();
    Mix_Chunk *m_chunk = nullptr;
    bool m_used_operator_new = false;
};
