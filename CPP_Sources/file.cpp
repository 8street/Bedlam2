#include <fstream>
#include <iostream>
#include <algorithm>

#include "file.h"

File::File()
{
    m_file_size = 0;
}

File::File(const std::string &path)
    : m_file(path)
{
    load_data();
}

int File::load(const std::string &path)
{
    const std::filesystem::path new_path = path;
    if (new_path == m_file && m_file_size)
    {
        return 0;
    }
    return set_path(path);
}

int File::set_path(const std::string &path)
{
    int retval = 0;
    const std::filesystem::path old_path = m_file;
    m_file = path;

    if (std::filesystem::exists(m_file))
    {
        retval = load_data();
    }
    else
    {
        m_file = old_path;
        retval = 1;
    }

    return retval;
}

std::string File::get_full_path() const
{
    return m_file.string();
}

std::string File::get_directory() const
{
    return m_file.parent_path().string();
}

std::string File::get_filename() const
{
    return m_file.filename().string();
}

std::string File::get_extension() const
{
    return m_file.extension().string();
}

int File::load_data()
{
    std::ifstream file;
    file.open(get_full_path().c_str(), std::ios_base::in | std::ios_base::binary);
    if (!file.is_open())
    {
        file.close();
        std::string path = get_full_path();
        std::transform(path.begin(), path.end(), path.begin(), ::toupper);
        m_file.assign(path);
        file.open(get_full_path().c_str(), std::ios_base::in | std::ios_base::binary);
        if (!file.is_open())
        {
            std::cout << "Unable to open file " << get_full_path().c_str() << std::endl;
            exit(404);
        }
    }

    // get filesize
    file.seekg(0, std::ios_base::end);
    const std::ifstream::pos_type file_end_pos = file.tellg();
    file.seekg(0, std::ios::beg);

    m_file_size = static_cast<size_t>(file_end_pos);

    m_data.clear();
    m_data.resize(m_file_size);

    file.read((char *)&m_data[0], static_cast<std::streamsize>(m_file_size));

    file.close();
    return 0;
}

uint16_t File::get_file_header() const
{
    return *((uint16_t *)m_data.data());
}

const uint8_t *File::get_ptr() const
{
    return m_data.data();
}

uint8_t *File::get_ptr()
{
    return m_data.data();
}

size_t File::get_size() const
{
    return m_file_size;
}
