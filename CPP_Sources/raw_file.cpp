#include "raw_file.h"

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