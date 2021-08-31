#include <fstream>
#include <iostream>

#include "raw_file.h"
#include "sdl_event.h"

RAW_File::RAW_File()
{
}

RAW_File::RAW_File(const std::string &path)
    : File(path)
{
    load_chunk();
}

RAW_File::~RAW_File()
{
    destroy();
}

RAW_File::RAW_File(const RAW_File &raw)
{
    load_raw(raw.get_full_path());
}

int RAW_File::load_raw(const std::string &path)
{
    int ret_val = 0;
    ret_val |= load(path);
    ret_val |= load_chunk();
    return ret_val;
}

const Mix_Chunk *RAW_File::get_chunk() const
{
    return m_raw_chunk;
}

Mix_Chunk *RAW_File::get_chunk()
{
    return m_raw_chunk;
}

int RAW_File::load_chunk()
{
    if (m_raw_chunk)
    {
        destroy();
    }
    if (get_ptr() && get_size())
    {
        m_raw_chunk = Mix_QuickLoad_RAW(get_ptr(), get_size());
    }
    if (!m_raw_chunk)
    {
        return -1;
    }
    return 0;
}

int RAW_File::destroy()
{
    if (m_raw_chunk)
    {
        Mix_FreeChunk(m_raw_chunk);
        m_raw_chunk = nullptr;
    }
    return 0;
}

int RAW_File::load_data()
{
    std::ifstream file;
    file.open(get_full_path().c_str(), std::ios_base::in | std::ios_base::binary);
    if (!file.is_open())
    {
        std::cout << "Unable to open file %s." << get_full_path().c_str() << std::endl;
        exit(404);
    }

    // get filesize
    file.seekg(0, std::ios_base::end);
    const std::ifstream::pos_type file_end_pos = file.tellg();
    file.seekg(0, std::ios::beg);

    m_file_size = static_cast<size_t>(file_end_pos);

    m_data.clear();

    char ch = 0;
    for (size_t i = 0; i < m_file_size; i++)
    {
        file.read(&ch, static_cast<std::streamsize>(1));
        // we need 2 push back because 2 channels is active
        m_data.push_back((uint8_t)ch);
        m_data.push_back((uint8_t)ch);
        if (i % 512 == 0)
        {
            SDL_events();
        }
    }

    m_file_size *= 2;

    file.close();
    return 0;
}