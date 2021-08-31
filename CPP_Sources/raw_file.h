#pragma once
#ifdef _MSC_VER
#    include <SDL_mixer.h>
#else
#    include <SDL2/SDL_mixer.h>
#endif
#include "file.h"

class RAW_File : public File
{
public:
    RAW_File();
    RAW_File(const std::string &path);
    ~RAW_File();
    RAW_File(const RAW_File &raw);

    int load_raw(const std::string &path);
    Mix_Chunk *get_chunk();
    const Mix_Chunk *get_chunk() const;

private:
    Mix_Chunk *m_raw_chunk = nullptr;
    int load_chunk();
    int destroy();
    virtual int load_data();
};