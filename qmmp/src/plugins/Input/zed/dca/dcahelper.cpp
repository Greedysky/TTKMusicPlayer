#include "dcahelper.h"

#include <qmmp/qmmp.h>

#ifdef WORDS_BIGENDIAN
#  define s16_LE(s16,channels) s16_swap (s16, channels)
#  define s16_BE(s16,channels) do {} while(0)
#  define s32_LE(s32,channels) s32_swap (s32, channels)
#  define s32_BE(s32,channels) do {} while(0)
#  define u32_LE(u32) ((((u32)&0xff000000)>>24)|(((u32)&0x00ff0000)>>8)|(((u32)&0x0000ff00)<<8)|(((u32)&0x000000ff)<<24))
#  define u16_LE(u16) ((((u16)&0xff00)>>8)|(((u16)&0x00ff)<<8))
#else
#  define s16_LE(s16,channels) do {} while(0)
#  define s16_BE(s16,channels) s16_swap(s16, channels)
#  define s32_LE(s32,channels) do {} while(0)
#  define s32_BE(s32,channels) s32_swap(s32, channels)
#  define u32_LE(u32) (u32)
#  define u16_LE(u16) (u16)
#endif

static int channel_remap[][7] = {
// DCA_MONO
    {0},
// DCA_CHANNEL
// DCA_STEREO
// DCA_STEREO_SUMDIFF
// DCA_STEREO_TOTAL
    {0,1},
    {0,1},
    {0,1},
    {0,1},
//DCA_3F
    {0,1,2},
//DCA_2F1R
    {0,1,2},
//DCA_3F1R
    {0,1,2,3},
//DCA_2F2R
    {0,1,2,3},
//DCA_3F2R
    {0,1,2,3,4},
//DCA_4F2R
    {0,1,2,3,4,5},

/// same with LFE

// DCA_MONO
    {0,1},
// DCA_CHANNEL
// DCA_STEREO
// DCA_STEREO_SUMDIFF
// DCA_STEREO_TOTAL
    {0,1,2},
    {0,1,2},
    {0,1,2},
    {0,1,2},
//DCA_3F
    {1,2,0,3},
//DCA_2F1R
    {0,1,3,2},
//DCA_3F1R
    {1,2,0,4,3},
//DCA_2F2R
    {0,1,4,2,3},
//DCA_3F2R
    {1,2,0,5,3,4},
//DCA_4F2R
    {1,2,5,3,4,6,7} // FL|FR|LFE|FLC|FRC|RL|RR
};

struct wavfmt_t
{
    uint16_t wFormatTag;
    uint16_t nChannels;
    uint32_t nSamplesPerSec;
    uint32_t nAvgBytesPerSec;
    uint16_t nBlockAlign;
    uint16_t wBitsPerSample;
    uint16_t cbSize;
};

static int64_t dts_open_wav(FILE *fp, wavfmt_t *fmt, int64_t *totalsamples)
{
    char riff[4];
    if(stdio_read(fp, &riff, 1, sizeof(riff)) != sizeof(riff))
    {
        return -1;
    }

    if(strncmp(riff, "RIFF", 4))
    {
        return -1;
    }

    uint32_t size;
    if(stdio_read(fp, &size, 1, sizeof(size)) != sizeof(size))
    {
        return -1;
    }
    size = u32_LE(size);

    char type[4];
    if(stdio_read(fp, type, 1, sizeof(type)) != sizeof(type))
    {
        return -1;
    }

    if(strncmp(type, "WAVE", 4))
    {
        return -1;
    }

    // fmt subchunk
    char fmtid[4];
    if(stdio_read(fp, fmtid, 1, sizeof(fmtid)) != sizeof(fmtid))
    {
        return -1;
    }

    if(strncmp(fmtid, "fmt ", 4))
    {
        return -1;
    }

    uint32_t fmtsize;
    if(stdio_read(fp, &fmtsize, 1, sizeof(fmtsize)) != sizeof(fmtsize))
    {
        return -1;
    }
    fmtsize = u32_LE(fmtsize);

    if(stdio_read(fp, fmt, 1, sizeof(wavfmt_t)) != sizeof(wavfmt_t))
    {
        return -1;
    }

    fmt->wFormatTag = u16_LE(fmt->wFormatTag);
    fmt->nChannels = u16_LE(fmt->nChannels);
    fmt->nSamplesPerSec = u32_LE(fmt->nSamplesPerSec);
    fmt->nAvgBytesPerSec = u32_LE(fmt->nAvgBytesPerSec);
    fmt->nBlockAlign = u16_LE(fmt->nBlockAlign);
    fmt->wBitsPerSample = u16_LE(fmt->wBitsPerSample);
    fmt->cbSize = u16_LE(fmt->cbSize);

    if(fmt->wFormatTag != 0x0001 || fmt->wBitsPerSample != 16)
    {
        return -1;
    }

    stdio_seek(fp, (int)fmtsize - (int)sizeof(wavfmt_t), SEEK_CUR);
    // data subchunk
    char data[4];
    if(stdio_read(fp, data, 1, sizeof(data)) != sizeof(data))
    {
        return -1;
    }

    if(strncmp(data, "data", 4))
    {
        return -1;
    }

    uint32_t datasize;
    if(stdio_read(fp, &datasize, 1, sizeof(datasize)) != sizeof(datasize))
    {
        return -1;
    }

    datasize = u32_LE(datasize);
    *totalsamples = datasize / ((fmt->wBitsPerSample >> 3) * fmt->nChannels);
    return stdio_tell(fp);
}

static int16_t convert(int32_t i)
{
#ifdef LIBDCA_FIXED
    i >>= 15;
#else
    i -= 0x43c00000;
#endif
    return (i > 32767) ? 32767 : ((i < -32768) ? -32768 : i);
}

static int convert_samples(decode_info *state, int)
{
    sample_t *samples = dca_samples(state->state);
    int16_t *dst = state->output_buffer + state->remaining * state->channels;

    for(int i = 0; i < 256; ++i)
    {
        for(int c = 0; c < state->channels; ++c)
        {
            *dst++ = convert(*((int32_t*)(samples + 256 * c)));
        }
        samples++;
    }

    state->remaining += 256;
    return 0;
}

static int dca_decode_data(decode_info *ddb_state, uint8_t *start, size_t size, int probe)
{
    size_t len;
    int n_decoded = 0;
    uint8_t *end = start + size;

    while(true)
    {
        len = end - start;
        if(!len)
        {
            break;
        }

        if(len > ddb_state->bufpos - ddb_state->bufptr)
        {
            len = ddb_state->bufpos - ddb_state->bufptr;
        }

        memcpy(ddb_state->bufptr, start, len);
        ddb_state->bufptr += len;
        start += len;

        if(ddb_state->bufptr == ddb_state->bufpos)
        {
            if(ddb_state->bufpos == ddb_state->buf + HEADER_SIZE)
            {
                const int length = dca_syncinfo(ddb_state->state, ddb_state->buf, &ddb_state->flags, &ddb_state->sample_rate, &ddb_state->bitrate, &ddb_state->frame_length);
                if(!length)
                {
                    for(ddb_state->bufptr = ddb_state->buf; ddb_state->bufptr < ddb_state->buf + HEADER_SIZE-1; ++ddb_state->bufptr)
                    {
                        ddb_state->bufptr[0] = ddb_state->bufptr[1];
                    }
                    continue;
                }
                else if(probe)
                {
                    return length;
                }
                ddb_state->bufpos = ddb_state->buf + length;
            }
            else
            {
                if(!ddb_state->disable_adjust)
                {
                    ddb_state->flags |= DCA_ADJUST_LEVEL;
                }

                constexpr sample_t bias = 384;
                level_t level = (level_t)ddb_state->gain;
                if(dca_frame(ddb_state->state, ddb_state->buf, &ddb_state->flags, &level, bias))
                {
                    goto error;
                }

                if(ddb_state->disable_dynrng)
                {
                    dca_dynrng(ddb_state->state, nullptr, nullptr);
                }

                for(int i = 0; i < dca_blocks_num(ddb_state->state); ++i)
                {
                    if(dca_block(ddb_state->state))
                    {
                        goto error;
                    }
                    // at this stage we can call dca_samples to get pointer to samples buffer
                    convert_samples(ddb_state, ddb_state->flags);
                    n_decoded += 256;
                }

                ddb_state->bufptr = ddb_state->buf;
                ddb_state->bufpos = ddb_state->buf + HEADER_SIZE;
                continue;
error:
                ddb_state->bufptr = ddb_state->buf;
                ddb_state->bufpos = ddb_state->buf + HEADER_SIZE;
            }
        }
    }
    return n_decoded;
}


DCAHelper::DCAHelper(const QString &path)
    : m_path(path)
{
    m_info = (decode_info*)calloc(sizeof(decode_info), 1);
}

DCAHelper::~DCAHelper()
{
    deinit();
}

void DCAHelper::deinit()
{
    if(m_info)
    {
        if(m_info->state)
        {
            dca_free(m_info->state);
        }

        if(m_info->file)
        {
           stdio_close(m_info->file);
        }
        free(m_info);
    }
}

bool DCAHelper::initialize()
{
    m_info->file = stdio_open(QmmpPrintable(m_path));
    if(!m_info->file)
    {
        qWarning("DCAHelper: open file failed");
        return false;
    }

    wavfmt_t fmt;
    int64_t total = -1;
    // WAV format
    if((m_info->offset = dts_open_wav(m_info->file, &fmt, &total)) == -1)
    {
        total = -1;
        m_info->offset = 0;
        m_info->bits_per_sample = 16;
    }
    else
    {
        m_info->bits_per_sample = fmt.wBitsPerSample;
        m_info->channels = fmt.nChannels;
        m_info->sample_rate = fmt.nSamplesPerSec;
        m_info->length = (float)total / fmt.nSamplesPerSec * 1000;
    }

    m_info->gain = 1;
    m_info->bufptr = m_info->buf;
    m_info->bufpos = m_info->buf + HEADER_SIZE;
    m_info->state = dca_init(0);

    if(!m_info->state)
    {
        qWarning("DCAHelper: dts_open_wav get state error");
        return false;
    }

    // prebuffer 1st piece, and get decoded samplerate and nchannels
    const size_t rd = stdio_read(m_info->file, m_info->inbuf, 1, BUFFER_SIZE);
    const int len = dca_decode_data(m_info, m_info->inbuf, rd, 1);
    if(!len)
    {
        qWarning("DCAHelper: dca_decode_data error");
        return false;
    }

    m_info->bufptr = m_info->buf;
    m_info->bufpos = m_info->buf + HEADER_SIZE;
    m_info->frame_byte_size = len;

    const int flags = m_info->flags &~ (DCA_LFE | DCA_ADJUST_LEVEL);
    switch(flags)
    {
    case DCA_MONO:
        qDebug("DCAHelper: mono");
        m_info->channels = 1;
        break;
    case DCA_CHANNEL:
    case DCA_STEREO:
    case DCA_DOLBY:
    case DCA_STEREO_SUMDIFF:
    case DCA_STEREO_TOTAL:
        qDebug("DCAHelper: stereo");
        m_info->channels = 2;
        break;
    case DCA_3F:
    case DCA_2F1R:
        qDebug("DCAHelper: 3F or 2F1R");
        m_info->channels = 3;
        break;
    case DCA_2F2R:
    case DCA_3F1R:
        qDebug("DCAHelper: 2F2R or 3F1R");
        m_info->channels = 4;
        break;
    case DCA_3F2R:
        qDebug("DCAHelper: 3F2R");
        m_info->channels = 5;
        break;
    case DCA_4F2R:
        qDebug("DCAHelper: 4F2R");
        m_info->channels = 6;
        break;
    }

    if(m_info->flags & DCA_LFE)
    {
        qDebug("DCAHelper: LFE");
        m_info->channels++;
    }

    if(!m_info->channels)
    {
        qWarning("DCAHelper: dts invalid numchannels");
        return false;
    }

    // calculate duration
    if(m_info->length <= 0)
    {
        total = stdio_length(m_info->file) / len * m_info->frame_length;
        m_info->length = (float)total / m_info->sample_rate * 1000;
    }

    m_info->end_sample = total - 1;
    return true;
}

void DCAHelper::seek(qint64 time)
{
    const int sample = time * sampleRate() / 1000;
    // calculate file offset from framesize / framesamples
    const int64_t nframe = sample / m_info->frame_length;
    const int64_t offs = m_info->frame_byte_size * nframe + m_info->offset;

    stdio_seek(m_info->file, offs, SEEK_SET);
    m_info->remaining = 0;
    m_info->samples_to_skip = (int)(sample - nframe * m_info->frame_length);
    m_info->current_sample = sample;
}

qint64 DCAHelper::read(unsigned char *data, qint64 maxSize)
{
    const int sampleSize = channels() * depth() / 8;
    if(m_info->end_sample >= 0)
    {
        if(m_info->current_sample + maxSize / sampleSize > m_info->end_sample)
        {
            maxSize = (int)((m_info->end_sample - m_info->current_sample + 1) * sampleSize);
            if(maxSize <= 0)
            {
                return 0;
            }
        }
    }

    const int initSize = maxSize;
    while(maxSize > 0)
    {
        if(m_info->samples_to_skip > 0 && m_info->remaining > 0)
        {
            const int skip = MIN(m_info->remaining, m_info->samples_to_skip);
            if(skip < m_info->remaining)
            {
                memmove(m_info->output_buffer, m_info->output_buffer + skip * channels(), (m_info->remaining - skip) * sampleSize);
            }

            m_info->remaining -= skip;
            m_info->samples_to_skip -= skip;
        }

        if(m_info->remaining > 0)
        {
            int n = maxSize / sampleSize;
            n = MIN(n, m_info->remaining);

            if(!(m_info->flags & DCA_LFE))
            {
                memcpy(data, m_info->output_buffer, n * sampleSize);
                data += n * sampleSize;
            }
            else
            {
                int chmap = (m_info->flags & DCA_CHANNEL_MASK) &~ DCA_LFE;
                if(m_info->flags & DCA_LFE)
                {
                    chmap += 11;
                }

                // remap channels
                char *in = (char *)m_info->output_buffer;
                for(int s = 0; s < n; ++s)
                {
                    for(int i = 0; i < channels(); ++i)
                    {
                        ((int16_t *)data)[i] = ((int16_t*)in)[channel_remap[chmap][i]];
                    }

                    in += sampleSize;
                    data += sampleSize;
                }
            }

            if(m_info->remaining > n)
            {
                memmove(m_info->output_buffer, m_info->output_buffer + n * channels(), (m_info->remaining - n) * sampleSize);
            }

            maxSize -= n * sampleSize;
            m_info->remaining -= n;
        }

        if(maxSize > 0 && !m_info->remaining)
        {
            const size_t rd = stdio_read(m_info->file, m_info->inbuf, 1, BUFFER_SIZE);
            if(!dca_decode_data(m_info, m_info->inbuf, rd, 0))
            {
                break;
            }
        }
    }

    m_info->current_sample += (initSize - maxSize) / sampleSize;
    return initSize - maxSize;
}
