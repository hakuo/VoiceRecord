#include "Media.h"
#include <fstream>
#include <iostream>
#include "FLAC++/encoder.h"
Media::Media(/* args */)
{
}

Media::~Media()
{
}

/*
 * @function:
 * @brief: 
 * @parameters: 
 * @return: filename
 * @last_modified: 03.16.2019
 */
std::string Media::get_file_name(std::string _base_name, enum file_type_t _type)
{
    return _base_name.append(get_file_extension(_type));
}

/*
 * @function:
 * @brief: 
 * @parameters: 
 * @return: 
 * @last_modified: 03.16.2019
 */
std::string Media::get_file_extension(enum file_type_t _type)
{
    std::string _ext;
    switch (_type)
    {
    case PCM:
        /* code */
        _ext = ".pcm";
        break;
    case FLAC:
        _ext = ".flac";
        break;
    default:
        break;
    }
    return _ext;
}

/*
 * @function:
 * @brief:
 * @parameter:
 * @return:
 * @last_modified: 03.16.2019
 */
std::string Media::remove_file_extension(std::string _filename)
{
    auto lastindex = _filename.find_last_of(".");
    if (lastindex == std::string::npos)
    {
        return _filename;
    }
    return _filename.substr(0, lastindex);
}

/*
 * @function:
 * @brief:
 * @parameter:
 * @return:
 * @last_modified: 03.16.2019
 */
std::string Media::change_file_extension(std::string _filename, enum file_type_t _type)
{
    return get_file_name(remove_file_extension(_filename), _type);
}

/*
 * @function:
 * @brief:
 * @parameter:
 * @return:
 * @last_modified: 03.16.2019
 */
int Media::get_file_size(std::string filename)
{
    std::ifstream file(filename.c_str(), std::ifstream::in | std::ifstream::binary);

    if (!file.is_open())
    {
        return -1;
    }

    file.seekg(0, std::ios::end);
    int fileSize = file.tellg();
    file.close();

    return fileSize;
}
/*
 * @function:
 * @brief: 
 * @parameters: 
 * @return: 
 * @last_modified: 03.16.2019
 */
void Media::write_pcm(std::string filename, char *buffer, int len)
{
    std::ofstream pcm_file;
    pcm_file.open(filename, std::ios::out | std::ios::app | std::ios::binary);
    if (pcm_file.is_open())
    {
        pcm_file.write(buffer, len);
        pcm_file.close();
    }
}

/*
 * @function:
 * @brief: 
 * @parameters: 
 * @return: filename
 * @last_modified: 03.16.2019
 */

bool Media::pcm_to_flac(std::string _pcm_file_name, std::string _flac_file_name)
{
    bool ok = true;
    FLAC::Encoder::File encoder;
    FLAC__byte buffer[BUFFER_SIZE];
    FLAC__int32 pcm[READSIZE * CHANNELS];   // FLAC size

    int total_samples = get_file_size(_pcm_file_name) / (BYTES_PER_SAMPLE * CHANNELS);
    ok &= encoder.set_verify(true);
    ok &= encoder.set_compression_level(5);
    ok &= encoder.set_channels(CHANNELS);
    ok &= encoder.set_bits_per_sample(BITS_PER_SAMPLE);
    ok &= encoder.set_sample_rate(SAMPLE_RATE);

    // initialize encoder
    if (ok)
    {
        auto init_status = encoder.init(_flac_file_name);
        if (init_status != FLAC__STREAM_ENCODER_INIT_STATUS_OK)
        {
            std::cout << "ERROR: initializing encoder: " << FLAC__StreamEncoderInitStatusString[init_status] << std::endl;
            ok = false;
        }
    }

    if (ok)
    {
        size_t left = (size_t)total_samples;
        std::ifstream pcmfile(_pcm_file_name.c_str(), std::ifstream::in | std::ifstream::binary);
        while (ok && left)
        {
            size_t need = (left > READSIZE) ? (size_t)READSIZE : (size_t)left;
            auto read_size = need * BYTES_PER_SAMPLE * CHANNELS;
            pcmfile.read((char *)buffer, read_size);
            size_t i;
            for (i = 0; i < need * CHANNELS; i++)
            {
                /* inefficient but simple and works on big- or little-endian machines */
                pcm[i] = (FLAC__int32)(((FLAC__int16)(FLAC__int8)buffer[2 * i + 1] << 8) | (FLAC__int16)buffer[2 * i]);
            }
            /* feed samples to encoder */
            ok = encoder.process_interleaved(pcm, need);

            left -= need;
        }
        pcmfile.close();
    }
    ok &= encoder.finish();

    return ok;
}

