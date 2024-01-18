#include "OrcPch.hpp"

#include "Audio/MusicBuffer.hpp"
#include "Audio/OpenAL.hpp"

#include "Engine/Debug.hpp"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include <sndfile.h>

namespace orc {

MusicBuffer::MusicBuffer(const FilePath& filePath)
{
    loadFromFile(filePath);
}

MusicBuffer::~MusicBuffer()
{
    alCall(alDeleteBuffers, BUFFER_COUNT, &m_audioIDS[0]);
}

bool MusicBuffer::loadFromFile(const FilePath& filePath)
{
    enum FormatType {
        Int16,
        Float,
        IMA4,
        MSADPCM
    };

    SF_INFO audioFileInfo;
    SNDFILE* audioFile = sf_open(filePath.string().c_str(), SFM_READ, &audioFileInfo);

    if (!audioFile)
    {
        ORC_ERROR("Couldn't open audio in {}\nreason: {}", filePath.string(), sf_strerror(audioFile));
        return 0;
    }

    if (audioFileInfo.frames < 1)
    {
        ORC_ERROR("Bad sample count in %s ({})", filePath.string(), audioFileInfo.frames);
        sf_close(audioFile);
        return 0;
    }

    enum FormatType sampleFormat = Int16;
    switch ((audioFileInfo.format & SF_FORMAT_SUBMASK))
    {
        case SF_FORMAT_PCM_24:
        case SF_FORMAT_PCM_32:
        case SF_FORMAT_FLOAT:
        case SF_FORMAT_DOUBLE:
        case SF_FORMAT_VORBIS:
        case SF_FORMAT_OPUS:
        case SF_FORMAT_ALAC_20:
        case SF_FORMAT_ALAC_24:
        case SF_FORMAT_ALAC_32:
        case 0x0080: case 0x0081: case 0x0082: //SF_FORMAT_MPEG_LAYER I, II, III
        {
            if (alCall(alIsExtensionPresent, "AL_EXT_FLOAT32"))
                sampleFormat = Float;

            break;
        }
        case SF_FORMAT_IMA_ADPCM:
        {
            if (audioFileInfo.channels <= 2 && (audioFileInfo.format & SF_FORMAT_TYPEMASK) == SF_FORMAT_WAV && alCall(alIsExtensionPresent, "AL_EXT_IMA4") && alCall(alIsExtensionPresent, "AL_SOFT_block_alignment"))
                sampleFormat = IMA4;

            break;
        }
        case SF_FORMAT_MS_ADPCM:
        {
            if (audioFileInfo.channels <= 2 && (audioFileInfo.format & SF_FORMAT_TYPEMASK) == SF_FORMAT_WAV && alCall(alIsExtensionPresent, "AL_SOFT_MSADPCM") && alCall(alIsExtensionPresent, "AL_SOFT_block_alignment"))
                sampleFormat = MSADPCM;

            break;
        }
    }

    ALint byteBlockAlign = 0;
    ALint splBlockAlign = 0;

    if (sampleFormat == IMA4 || sampleFormat == MSADPCM)
    {
        SF_CHUNK_INFO inf = { "fmt ", 4, 0, NULL };
        SF_CHUNK_ITERATOR* iter = sf_get_chunk_iterator(audioFile, &inf);

        if (!iter || sf_get_chunk_size(iter, &inf) != SF_ERR_NO_ERROR || inf.datalen < 14)
        {
            sampleFormat = Int16;
        }
        else
        {
            ALubyte* fmtbuf = new ALubyte[inf.datalen]{};
            inf.data = fmtbuf;
            if (sf_get_chunk_data(iter, &inf) != SF_ERR_NO_ERROR)
            {
                sampleFormat = Int16;
            }
            else
            {
                byteBlockAlign = fmtbuf[12] | (fmtbuf[13] << 8);
                if (sampleFormat == IMA4)
                {
                    splBlockAlign = (byteBlockAlign / audioFileInfo.channels - 4) / 4 * 8 + 1;
                    if (splBlockAlign < 1 || ((splBlockAlign - 1) / 2 + 4) * audioFileInfo.channels != byteBlockAlign)
                        sampleFormat = Int16;
                }
                else
                {
                    splBlockAlign = (byteBlockAlign / audioFileInfo.channels - 7) * 2 + 2;
                    if (splBlockAlign < 2 || ((splBlockAlign - 2) / 2 + 7) * audioFileInfo.channels != byteBlockAlign)
                        sampleFormat = Int16;
                }
            }

            delete[] fmtbuf;
        }
    }

    if (sampleFormat == Int16)
    {
        splBlockAlign = 1;
        byteBlockAlign = audioFileInfo.channels * 2;
    }
    else if (sampleFormat == Float)
    {
        splBlockAlign = 1;
        byteBlockAlign = audioFileInfo.channels * 4;
    }

    ALenum format = AL_NONE;
    if (audioFileInfo.channels == 1)
    {
        if (sampleFormat == Int16)
            format = AL_FORMAT_MONO16;
        else if (sampleFormat == Float)
            format = AL_FORMAT_MONO_FLOAT32;
        else if (sampleFormat == IMA4)
            format = AL_FORMAT_MONO_IMA4;
        else if (sampleFormat == MSADPCM)
            format = AL_FORMAT_MONO_MSADPCM_SOFT;
    }
    else if (audioFileInfo.channels == 2)
    {
        if (sampleFormat == Int16)
            format = AL_FORMAT_STEREO16;
        else if (sampleFormat == Float)
            format = AL_FORMAT_STEREO_FLOAT32;
        else if (sampleFormat == IMA4)
            format = AL_FORMAT_STEREO_IMA4;
        else if (sampleFormat == MSADPCM)
            format = AL_FORMAT_STEREO_MSADPCM_SOFT;
    }
    else if (audioFileInfo.channels == 3)
    {
        if (sf_command(audioFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
        {
            if (sampleFormat == Int16)
                format = AL_FORMAT_BFORMAT2D_16;
            else if (sampleFormat == Float)
                format = AL_FORMAT_BFORMAT2D_FLOAT32;
        }
    }
    else if (audioFileInfo.channels == 4)
    {
        if (sf_command(audioFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
        {
            if (sampleFormat == Int16)
                format = AL_FORMAT_BFORMAT3D_16;
            else if (sampleFormat == Float)
                format = AL_FORMAT_BFORMAT3D_FLOAT32;
        }
    }

    if (!format)
    {
        ORC_ERROR("Unsupported channel count: {}", audioFileInfo.channels);
        sf_close(audioFile);
        return 0;
    }

    if (audioFileInfo.frames / splBlockAlign > (sf_count_t)(INT_MAX / byteBlockAlign))
    {
        ORC_ERROR("Too many samples in {} ({})", filePath.string(), audioFileInfo.frames);
        sf_close(audioFile);
        return 0;
    }

    sf_count_t framesCount = 0;

    void* memoryBuffer = malloc((size_t)(audioFileInfo.frames / splBlockAlign * byteBlockAlign));
    if (sampleFormat == Int16)
    {
        framesCount = sf_readf_short(audioFile, (short*)memoryBuffer, audioFileInfo.frames);
    }
    else if (sampleFormat == Float)
    {
        framesCount = sf_readf_float(audioFile, (float*)memoryBuffer, audioFileInfo.frames);
    }
    else
    {
        sf_count_t count = audioFileInfo.frames / splBlockAlign * byteBlockAlign;
        framesCount = sf_read_raw(audioFile, memoryBuffer, count);
        if (framesCount > 0)
            framesCount = framesCount / byteBlockAlign * splBlockAlign;
    }
    if (framesCount < 1)
    {
        free(memoryBuffer);
        sf_close(audioFile);
        ORC_ERROR("Failed to read samples in {} ({})", filePath.string(), framesCount);
        return 0;
    }

    
    alCall(alDeleteBuffers, BUFFER_COUNT, &m_audioIDS[0]);
    m_buffer = std::vector<short>(BUFFER_SIZE * audioFileInfo.channels);

    ALuint buffers[BUFFER_COUNT] = {};
    alCall(alGenBuffers, BUFFER_COUNT, &buffers[0]);

    for (uint32_t i = 0; i < BUFFER_COUNT; i++)
    {
        if (splBlockAlign > 1)
        {
            alCall(alBufferi, buffers[i], AL_UNPACK_BLOCK_ALIGNMENT_SOFT, splBlockAlign);
        }

        alCall(alBufferData, buffers[i], format, memoryBuffer, (ALsizei)(framesCount / splBlockAlign * byteBlockAlign), audioFileInfo.samplerate);

        m_audioIDS[i] = buffers[i];
    }

    free(memoryBuffer);
    sf_close(audioFile);

    return 1;
}

}
