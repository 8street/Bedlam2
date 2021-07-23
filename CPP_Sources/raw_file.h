#pragma once
#include <SDL_mixer.h>
#include "file.h"


class RAW_File : public File
{
public:
    RAW_File();
    RAW_File(const std::string &path);
    ~RAW_File();

    int load_raw(const std::string &path);
    Mix_Chunk *get_chunk();
    const Mix_Chunk *get_chunk() const;

private:
    Mix_Chunk *m_raw_chunk = nullptr; 
    int load_chunk();
    int destroy();
};