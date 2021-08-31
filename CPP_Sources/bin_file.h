#pragma once
#include "file.h"

class BIN_File : public File
{
public:
    BIN_File();
    BIN_File(const std::string &path);

    int get_img_count() const;
    int get_img_height(int img) const;
    int get_img_width(int img) const;
    int get_img_x_offset(int img) const;
    int get_img_y_offset(int img) const;

    uint8_t *get_img_header_ptr(int img);
    uint8_t *get_img_data_ptr(int img);
    const uint8_t *get_img_header_ptr(int img) const;
    const uint8_t *get_img_data_ptr(int img) const;

private:
};
