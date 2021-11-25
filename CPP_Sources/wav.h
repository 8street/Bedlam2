#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "file.h"

#define PCM_SIZE 16
#define PCM 1

struct WAV_header
{
    /* RIFF Chunk Descriptor */
    uint8_t riff[4] = { 'R', 'I', 'F', 'F' }; // RIFF Header Magic header
    uint32_t chunkSize = 0;                   // RIFF Chunk Size (36 + subchunk2Size)
    uint8_t wave[4] = { 'W', 'A', 'V', 'E' }; // WAVE Header
    /* "fmt" sub-chunk */
    uint8_t fmt[4] = { 'f', 'm', 't', ' ' }; // FMT header
    uint32_t subchunk1Size = PCM_SIZE;       // Size of the fmt chunk
    uint16_t audioFormat = PCM;              // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM
                                             // Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t numChannels = 0;                // Number of channels 1=Mono 2=Sterio
    uint32_t sampleRate = 0;                 // Sampling Frequency in Hz
    uint32_t bytesPerSec = 0;                // (sampleRate * numChannels * bitsPerSample/8)
    uint16_t blockAlign = 0;                 // (numChannels * bitsPerSample/8)
    uint16_t bitsPerSample = 0;              // Number of bits per sample
    /* "data" sub-chunk */
    uint8_t subchunk2ID[4] = { 'd', 'a', 't', 'a' }; // "data"  string
    uint32_t subchunk2Size = 0;                      // Sampled data length
};

class WAV
{
public:
    WAV();
    WAV(const uint8_t *const raw_ptr, size_t data_size, uint32_t samplerate = 11025, uint32_t bits_per_sample = 8,
        uint32_t num_channels = 1);
    WAV(const File &file, uint32_t samplerate = 11025, uint32_t bits_per_sample = 8, uint32_t num_channels = 1);
    WAV(const std::string &path);
    ~WAV();

    int load_from_path(const std::string &path);
    int load_from_mem(
        const uint8_t *const raw_ptr, size_t data_size, uint32_t samplerate = 11025, uint32_t bits_per_sample = 8,
        uint32_t num_channels = 1);
    const uint8_t *get_data() const;
    size_t get_size() const;

private:
    int fill_wav_header(
        WAV_header &header, size_t data_size, uint32_t samplerate, uint32_t bits_per_sample, uint32_t num_channels);
    int load_wav_data(const uint8_t *const raw_ptr, size_t data_size);

    std::vector<uint8_t> m_wav_data;
};
