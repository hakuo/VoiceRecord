#pragma once

#include <iostream>

class Media
{
  private:
    /* data */
  public:
    Media(/* args */);
    ~Media();
    enum file_type_t
    {
        PCM = 0,
        FLAC
    };
    // config media
    static const int CHANNELS = 1;                                           // Number of channels
    static const int BYTES_PER_SAMPLE = 2;                                   // Bytes per sample
    static const int BITS_PER_SAMPLE = 8 * BYTES_PER_SAMPLE;                 // Bits per sample
    static const int SAMPLE_RATE = 44100;                                    // Sample rate
    static const int READSIZE = 1024;                                        // Number of samples in one read
    static const int BUFFER_SIZE = (READSIZE * BYTES_PER_SAMPLE * CHANNELS); // The read buffer size
    
    // functions
    static void write_pcm(std::string filename, char *buffer, int len);
    static bool pcm_to_flac(std::string pcm_file, std::string flac_file);
    static std::string get_file_name(std::string _base_name, enum file_type_t _type);
    static std::string get_file_extension(enum file_type_t _type);
    static int get_file_size(std::string filename);
    static std::string remove_file_extension(std::string _filename);
    static std::string change_file_extension(std::string _filename, enum file_type_t _type);
};