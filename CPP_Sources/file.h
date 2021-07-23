#pragma once

#include <filesystem>
#include <stdint.h>
#include <string>
#include <vector>

class File
{
public:
    File();
    File(const std::string &path);

    int load(const std::string &path);
    const uint8_t *get_ptr() const;
    uint8_t *get_ptr();
    std::string get_full_path() const;
    std::string get_directory() const;
    std::string get_filename() const;
    std::string get_extension() const;
    uint16_t get_file_header() const;
    size_t get_size() const;

private:
    virtual int load_data();
    int set_path(const std::string &path);

protected:
    std::filesystem::path m_file;
    std::vector<uint8_t> m_data;
    size_t m_file_size;
};
