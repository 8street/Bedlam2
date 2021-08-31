#include "bin_file.h"
#include "helper.h"
#include <iostream>

BIN_File::BIN_File()
{
}

BIN_File::BIN_File(const std::string &path)
    : File(path)
{
}

int BIN_File::get_img_count() const
{
    return get_file_header();
}

int BIN_File::get_img_height(int img) const
{
    const uint8_t *img_ptr = get_img_header_ptr(img);
    const uint16_t img_header = *(uint16_t *)img_ptr;
    if (img_ptr)
    {
        if (img_header & 0b10)
        {
            img_ptr += 4;
        }
        img_ptr += 4;
        return *(uint16_t *)img_ptr;
    }
    return 0;
}

int BIN_File::get_img_width(int img) const
{
    const uint8_t *img_ptr = get_img_header_ptr(img);
    const uint16_t img_header = *(uint16_t *)img_ptr;
    if (img_ptr)
    {
        if (img_header & 0b10)
        {
            img_ptr += 4;
        }
        img_ptr += 2;
        return *(uint16_t *)img_ptr;
    }
    return 0;
}

int BIN_File::get_img_x_offset(int img) const
{
    const uint8_t *img_ptr = get_img_header_ptr(img);

    if (img_ptr)
    {
        const uint16_t img_header = *(uint16_t *)img_ptr;
        if (img_header & 0b10)
        {
            img_ptr += 4;
            return *(uint16_t *)img_ptr;
        }
    }
    return 0;
}

int BIN_File::get_img_y_offset(int img) const
{
    const uint8_t *img_ptr = get_img_header_ptr(img);
    if (img_ptr)
    {
        const uint16_t img_header = *(uint16_t *)img_ptr;
        if (img_header & 0b10)
        {
            img_ptr += 2;
            return *(uint16_t *)img_ptr;
        }
    }
    return 0;
}

uint8_t *BIN_File::get_img_header_ptr(int img)
{
    if (img > UINT16_MAX || img > get_img_count() - 1)
    {
        std::cout << "ERROR: img > img count: " << img << " > " << get_img_count() - 1;
        exit(405);
    }
    const uint32_t header_size = 2;
    uint8_t *bin_ptr = get_ptr();
    const uint32_t img_addr = *(uint32_t *)&bin_ptr[4 * img + header_size];
    return &bin_ptr[4 * img + header_size + img_addr];
}

uint8_t *BIN_File::get_img_data_ptr(int img)
{
    return get_img_header_ptr(img) + 10;
}

const uint8_t *BIN_File::get_img_header_ptr(int img) const
{
    if (img > UINT16_MAX || img > get_img_count() - 1)
    {
        std::cout << "ERROR: img > img count: " << img << " > " << get_img_count() - 1;
        exit(405);
    }
    const uint32_t header_size = 2;
    const uint8_t *bin_ptr = get_ptr();
    const uint32_t img_addr = *(uint32_t *)&bin_ptr[4 * img + header_size];
    return &bin_ptr[4 * img + header_size + img_addr];
}

const uint8_t *BIN_File::get_img_data_ptr(int img) const
{
    return get_img_header_ptr(img) + 10;
}
