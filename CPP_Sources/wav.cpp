#include <iostream>

#include "helper.h"
#include "wav.h"

WAV::WAV()
{
}

WAV::WAV(const uint8_t *const raw_ptr, size_t data_size, uint32_t samplerate, uint32_t bits_per_sample, uint32_t num_channels)
{
    load_from_mem(raw_ptr, data_size, samplerate, bits_per_sample, num_channels);
}

WAV::WAV(const File &file, uint32_t samplerate, uint32_t bits_per_sample, uint32_t num_channels)
{
    load_from_mem(file.get_ptr(), file.get_size(), samplerate, bits_per_sample, num_channels);
}

WAV::WAV(const std::string &path)
{
    load_from_path(path);
}

WAV::~WAV()
{
}

int WAV::load_from_path(const std::string &path)
{
    int ret_val = 0;
    const File file(path);
    const std::string file_extension = to_lower(file.get_extension());
    if (file_extension == ".raw")
    {
        load_from_mem(file.get_ptr(), file.get_size(), 11025, 8, 1);
    }
    else if (file_extension == ".wav")
    {
        load_wav_data(file.get_ptr(), file.get_size());
    }
    else
    {
        std::cout << "Unknown wave format: " << file.get_full_path() << std::endl;
    }
    return 0;
}

const uint8_t *WAV::get_data() const
{
    return m_wav_data.data();
}

size_t WAV::get_size() const
{
    return m_wav_data.size();
}

int WAV::fill_wav_header(
    WAV_header &header, size_t data_size, uint32_t samplerate, uint32_t bits_per_sample, uint32_t num_channels)
{
    if (samplerate > 384000 || bits_per_sample > 32 || num_channels > 8)
    {
        std::cout << "Invalid wav parameters. Samplerate: " << samplerate;
        std::cout << " Bits per sample: " << bits_per_sample;
        std::cout << " Num channels: " << num_channels << std::endl;
        return -1;
    }
    header.sampleRate = samplerate;
    header.bitsPerSample = bits_per_sample;
    header.numChannels = num_channels;
    header.bytesPerSec = header.sampleRate * header.numChannels * header.bitsPerSample / 8;
    header.blockAlign = header.numChannels * header.bitsPerSample / 8;
    header.subchunk2Size = data_size;
    header.chunkSize = 36 + header.subchunk2Size;
    return 0;
}

int WAV::load_from_mem(
    const uint8_t *const raw_ptr, size_t data_size, uint32_t samplerate, uint32_t bits_per_sample, uint32_t num_channels)
{
    int ret_val = 0;
    if (!raw_ptr || data_size == 0)
    {
        return -1;
    }
    WAV_header header;
    m_wav_data.clear();
    m_wav_data.reserve(sizeof(WAV_header) + data_size);
    ret_val |= fill_wav_header(header, data_size, samplerate, bits_per_sample, num_channels);
    const uint8_t *const header_ptr = reinterpret_cast<uint8_t *>(&header);
    m_wav_data.insert(m_wav_data.end(), header_ptr, header_ptr + sizeof(WAV_header));
    m_wav_data.insert(m_wav_data.end(), raw_ptr, raw_ptr + data_size);
    return ret_val;
}

int WAV::load_wav_data(const uint8_t *const raw_ptr, size_t data_size)
{
    if (!raw_ptr || data_size == 0)
    {
        return -1;
    }
    m_wav_data.clear();
    m_wav_data.reserve(data_size);
    m_wav_data.insert(m_wav_data.end(), raw_ptr, raw_ptr + data_size);
    return 0;
}