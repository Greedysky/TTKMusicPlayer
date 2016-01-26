/***************************************************************************
 *  Based on mq3 and madplay projects                                      *
 *                                                                         *
 * Copyright (c) 2000-2001 Brad Hughes <bhughes@trolltech.com>             *
 * Copyright (C) 2000-2004 Robert Leslie <rob@mars.org>                    *
 * Copyright (C) 2009-2013 Ilya Kotov forkotov02@hotmail.ru                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <taglib/id3v2header.h>
#include <taglib/tbytevector.h>
#include <math.h>
#include <stdio.h>
#include "tagextractor.h"
#include "decoder_mad.h"

#define XING_MAGIC (('X' << 24) | ('i' << 16) | ('n' << 8) | 'g')
#define INPUT_BUFFER_SIZE (32*1024)
#define USE_DITHERING

DecoderMAD::DecoderMAD(QIODevice *i) : Decoder(i)
{
    m_inited = false;
    m_totalTime = 0;
    m_channels = 0;
    m_bitrate = 0;
    m_freq = 0;
    m_len = 0;
    m_input_buf = 0;
    m_input_bytes = 0;
    m_output_bytes = 0;
    m_output_at = 0;
    m_skip_frames = 0;
    m_eof = false;

    m_left_dither.random = 0;
    m_left_dither.error[0] = 0;
    m_left_dither.error[1] = 0;
    m_left_dither.error[2] = 0;

    m_right_dither.random = 0;
    m_right_dither.error[0] = 0;
    m_right_dither.error[1] = 0;
    m_right_dither.error[2] = 0;
}

DecoderMAD::~DecoderMAD()
{
    deinit();
    if (m_input_buf)
    {
        qDebug("DecoderMAD: deleting input_buf");
        delete [] m_input_buf;
        m_input_buf = 0;
    }
}

bool DecoderMAD::initialize()
{
    m_inited = false;
    m_totalTime = 0;
    m_channels = 0;
    m_bitrate = 0;
    m_freq = 0;
    m_len = 0;
    m_input_bytes = 0;
    m_output_bytes = 0;
    m_output_at = 0;

    if (!input())
    {
        qWarning("DecoderMAD: cannot initialize.  No input.");
        return false;
    }

    if (!m_input_buf)
        m_input_buf = new char[INPUT_BUFFER_SIZE];

    if (!input()->isOpen())
    {
        if (!input()->open(QIODevice::ReadOnly))
        {
            qWarning("DecoderMAD: %s", qPrintable(input()->errorString ()));
            return false;
        }
    }

    if (input()->isSequential ()) //for streams only
    {
        TagExtractor extractor(input());
        if(!extractor.id3v2tag().isEmpty())
            addMetaData(extractor.id3v2tag());
    }

    mad_stream_init(&m_stream);
    mad_frame_init(&m_frame);
    mad_synth_init(&m_synth);

    if (!findHeader())
    {
        qDebug("DecoderMAD: Can't find a valid MPEG header.");
        return false;
    }
    mad_stream_buffer(&m_stream, (unsigned char *) m_input_buf, m_input_bytes);
    m_stream.error = MAD_ERROR_BUFLEN;
    mad_frame_mute (&m_frame);
    m_stream.next_frame = 0;
    m_stream.sync = 0;
    ChannelMap map;
    if(m_channels == 1)
        map << Qmmp::CHAN_FRONT_LEFT;
    else
        map << Qmmp::CHAN_FRONT_LEFT << Qmmp::CHAN_FRONT_RIGHT;
    configure(m_freq, map, Qmmp::PCM_S16LE);
    m_inited = true;
    return true;
}


void DecoderMAD::deinit()
{
    if (!m_inited)
        return;

    mad_synth_finish(&m_synth);
    mad_frame_finish(&m_frame);
    mad_stream_finish(&m_stream);

    m_inited = false;
    m_totalTime = 0;
    m_channels = 0;
    m_bitrate = 0;
    m_freq = 0;
    m_len = 0;
    m_input_bytes = 0;
    m_output_bytes = 0;
    m_output_at = 0;
    m_skip_frames = 0;
    m_eof = false;
}

bool DecoderMAD::findXingHeader(struct mad_bitptr ptr, unsigned int bitlen)
{
    if (bitlen < 64 || mad_bit_read(&ptr, 32) != XING_MAGIC)
        goto fail;

    xing.flags = mad_bit_read(&ptr, 32);
    bitlen -= 64;

    if (xing.flags & XING_FRAMES)
    {
        if (bitlen < 32)
            goto fail;

        xing.frames = mad_bit_read(&ptr, 32);
        bitlen -= 32;
    }

    if (xing.flags & XING_BYTES)
    {
        if (bitlen < 32)
            goto fail;

        xing.bytes = mad_bit_read(&ptr, 32);
        bitlen -= 32;
    }

    if (xing.flags & XING_TOC)
    {
        int i;

        if (bitlen < 800)
            goto fail;

        for (i = 0; i < 100; ++i)
            xing.toc[i] = mad_bit_read(&ptr, 8);

        bitlen -= 800;
    }

    if (xing.flags & XING_SCALE)
    {
        if (bitlen < 32)
            goto fail;

        xing.scale = mad_bit_read(&ptr, 32);
        bitlen -= 32;
    }

    return true;

fail:
    xing.flags = 0;
    xing.frames = 0;
    xing.bytes = 0;
    xing.scale = 0;
    return false;
}

bool DecoderMAD::findHeader()
{
    bool result = false;
    int count = 0;
    bool has_xing = false;
    bool is_vbr = false;
    mad_timer_t duration = mad_timer_zero;
    struct mad_header header;
    mad_header_init (&header);

    forever
    {
        m_input_bytes = 0;
        if (m_stream.error == MAD_ERROR_BUFLEN || !m_stream.buffer)
        {
            size_t remaining = 0;

            if (!m_stream.next_frame)
            {
                remaining = m_stream.bufend - m_stream.next_frame;
                memmove (m_input_buf, m_stream.next_frame, remaining);
            }

            m_input_bytes = input()->read(m_input_buf + remaining, INPUT_BUFFER_SIZE - remaining);

            if (m_input_bytes <= 0)
                break;

            mad_stream_buffer(&m_stream, (unsigned char *) m_input_buf + remaining, m_input_bytes);
            m_stream.error = MAD_ERROR_NONE;
        }

        if (mad_header_decode(&header, &m_stream) < 0)
        {
            if(m_stream.error == MAD_ERROR_LOSTSYNC)
            {
                uint tagSize = findID3v2((uchar *)m_stream.this_frame,
                                         (ulong) (m_stream.bufend - m_stream.this_frame));
                if (tagSize > 0)
                    mad_stream_skip(&m_stream, tagSize);
                continue;
            }
            else if (m_stream.error == MAD_ERROR_BUFLEN || MAD_RECOVERABLE(m_stream.error))
                continue;
            else
            {
                qDebug ("DecoderMAD: Can't decode header: %s", mad_stream_errorstr(&m_stream));
                break;
            }
        }
        result = true;

        if (input()->isSequential())
            break;

        count ++;
        //try to detect xing header
        if (count == 1)
        {
            m_frame.header = header;
            if (mad_frame_decode(&m_frame, &m_stream) != -1 &&
                    findXingHeader(m_stream.anc_ptr, m_stream.anc_bitlen))
            {
                is_vbr = true;

                qDebug ("DecoderMAD: Xing header detected");

                if (xing.flags & XING_FRAMES)
                {
                    has_xing = true;
                    count = xing.frames;
                    break;
                }
            }
        }
        //try to detect VBR
        if (!is_vbr && !(count > 15))
        {
            if (m_bitrate && header.bitrate != m_bitrate)
            {
                qDebug ("DecoderMAD: VBR detected");
                is_vbr = true;
            }
            else
                m_bitrate = header.bitrate;
        }
        else if (!is_vbr)
        {
            qDebug ("DecoderMAD: Fixed rate detected");
            break;
        }
        mad_timer_add (&duration, header.duration);
    }

    if (!result)
        return false;

    if (!is_vbr && !input()->isSequential())
    {
        double time = (input()->size() * 8.0) / (header.bitrate);
        double timefrac = (double)time - ((long)(time));
        mad_timer_set(&duration, (long)time, (long)(timefrac*100), 100);
    }
    else if (has_xing)
    {
        mad_timer_multiply (&header.duration, count);
        duration = header.duration;
    }

    m_totalTime = mad_timer_count(duration, MAD_UNITS_MILLISECONDS);
    qDebug ("DecoderMAD: Total time: %ld", long(m_totalTime));
    m_freq = header.samplerate;
    m_channels = MAD_NCHANNELS(&header);
    m_bitrate = header.bitrate / 1000;
    mad_header_finish(&header);
    input()->seek(0);
    m_input_bytes = 0;
    return true;
}

qint64 DecoderMAD::totalTime()
{
    if (!m_inited)
        return 0;
    return m_totalTime;
}

int DecoderMAD::bitrate()
{
    return int(m_bitrate);
}

qint64 DecoderMAD::read(char *data, qint64 size)
{
    if(decodeFrame())
        return madOutput(data, size);
    return 0;
}

qint64 DecoderMAD::read(float *data, qint64 samples)
{
    if(decodeFrame())
        return madOutputFloat(data, samples);
    return 0;
}

void DecoderMAD::seek(qint64 pos)
{
    if(m_totalTime > 0)
    {
        qint64 seek_pos = qint64(pos * input()->size() / m_totalTime);
        input()->seek(seek_pos);
        mad_frame_mute(&m_frame);
        mad_synth_mute(&m_synth);
        m_stream.error = MAD_ERROR_BUFLEN;
        m_stream.sync = 0;
        m_input_bytes = 0;
        m_stream.next_frame = 0;
        m_skip_frames = 2;
    }
}

bool DecoderMAD::fillBuffer()
{
    if (m_stream.next_frame)
    {
        m_input_bytes = &m_input_buf[m_input_bytes] - (char *) m_stream.next_frame;
        memmove(m_input_buf, m_stream.next_frame, m_input_bytes);
    }
    int len = input()->read((char *) m_input_buf + m_input_bytes, INPUT_BUFFER_SIZE - m_input_bytes);
    if (!len)
    {
        qDebug("DecoderMAD: end of file");
        return false;
    }
    else if(len < 0)
    {
        qWarning("DecoderMAD: error");
        return false;
    }
    m_input_bytes += len;
    mad_stream_buffer(&m_stream, (unsigned char *) m_input_buf, m_input_bytes);
    return true;
}

uint DecoderMAD::findID3v2(uchar *data, ulong size) //retuns ID3v2 tag size
{
    if (size < 10)
        return 0;

    if (((data[0] == 'I' && data[1] == 'D' && data[2] == '3') || //ID3v2 tag
         (data[0] == '3' && data[1] == 'D' && data[2] == 'I')) && //ID3v2 footer
            data[3] < 0xff && data[4] < 0xff && data[6] < 0x80 &&
            data[7] < 0x80 && data[8] < 0x80 && data[9] < 0x80)
    {
        TagLib::ByteVector byteVector((char *)data, size);
        TagLib::ID3v2::Header header(byteVector);
        return header.tagSize();
    }
    return 0;
}

unsigned long DecoderMAD::prng(unsigned long state) // 32-bit pseudo-random number generator
{
    return (state * 0x0019660dL + 0x3c6ef35fL) & 0xffffffffL;
}

long DecoderMAD::audio_linear_dither(unsigned int bits, mad_fixed_t sample,
                                     struct audio_dither *dither)
{
    unsigned int scalebits;
    mad_fixed_t output, mask, random;

    /* noise shape */
    sample += dither->error[0] - dither->error[1] + dither->error[2];

    dither->error[2] = dither->error[1];
    dither->error[1] = dither->error[0] / 2;

    /* bias */
    output = sample + (1L << (MAD_F_FRACBITS + 1 - bits - 1));

    scalebits = MAD_F_FRACBITS + 1 - bits;
    mask = (1L << scalebits) - 1;

    /* dither */
    random  = prng(dither->random);
    output += (random & mask) - (dither->random & mask);

    dither->random = random;

    /* clip */
    if (output > CLIP_MAX)
    {
        output = CLIP_MAX;

        if (sample > CLIP_MAX)
            sample = CLIP_MAX;
    }
    else if (output < CLIP_MIN)
    {
        output = CLIP_MIN;

        if (sample < CLIP_MIN)
            sample = CLIP_MIN;
    }

    /* quantize */
    output &= ~mask;

    /* error feedback */
    dither->error[0] = sample - output;

    /* scale */
    return output >> scalebits;
}

//generic linear sample quantize routine
long DecoderMAD::audio_linear_round(unsigned int bits, mad_fixed_t sample)
{
    /* round */
    sample += (1L << (MAD_F_FRACBITS - bits));

    /* clip */
    if (sample > CLIP_MAX)
        sample = CLIP_MAX;
    else if (sample < CLIP_MIN)
        sample = CLIP_MIN;

    /* quantize and scale */
    return sample >> (MAD_F_FRACBITS + 1 - bits);
}

bool DecoderMAD::decodeFrame()
{
    forever
    {
        if(((m_stream.error == MAD_ERROR_BUFLEN) || !m_stream.buffer) && !m_eof)
        {
            m_eof = !fillBuffer();
        }
        if(mad_frame_decode(&m_frame, &m_stream) < 0)
        {
            switch((int) m_stream.error)
            {
            case MAD_ERROR_LOSTSYNC:
            {
                //skip ID3v2 tag
                uint tagSize = findID3v2((uchar *)m_stream.this_frame,
                                         (ulong) (m_stream.bufend - m_stream.this_frame));
                if (tagSize > 0)
                {
                    mad_stream_skip(&m_stream, tagSize);
                    qDebug("DecoderMAD: %d bytes skipped", tagSize);
                }
                continue;
            }
            case MAD_ERROR_BUFLEN:
                if(m_eof)
                    return false;
                continue;
            default:
                if (!MAD_RECOVERABLE(m_stream.error))
                    return false;
                else
                    continue;
            }
        }
        if(m_skip_frames)
        {
            m_skip_frames--;
            continue;
        }
        mad_synth_frame(&m_synth, &m_frame);
        break;
    }
    return true;
}

qint64 DecoderMAD::madOutput(char *data, qint64 size)
{
    unsigned int samples_per_channel, channels;
    mad_fixed_t const *left, *right;

    samples_per_channel = m_synth.pcm.length;
    channels = m_synth.pcm.channels;
    left = m_synth.pcm.samples[0];
    right = m_synth.pcm.samples[1];
    m_bitrate = m_frame.header.bitrate / 1000;
    m_output_at = 0;
    m_output_bytes = 0;

    if(samples_per_channel * channels * 2 > size)
    {
        qWarning("DecoderMad: input buffer is too small");
        samples_per_channel = size / channels / 2;
    }

    while (samples_per_channel--)
    {
        signed int sample;
#ifdef USE_DITHERING
        sample = audio_linear_dither(16, *left++,  &m_left_dither);
#else
        sample = audio_linear_round(16, *left++);
#endif
        *(data + m_output_at++) = ((sample >> 0) & 0xff);
        *(data + m_output_at++) = ((sample >> 8) & 0xff);
        m_output_bytes += 2;

        if (channels == 2)
        {
#ifdef USE_DITHERING
            sample = audio_linear_dither(16, *right++, &m_right_dither);
#else
            sample = audio_linear_round(16, *right++);
#endif
            *(data + m_output_at++) = ((sample >> 0) & 0xff);
            *(data + m_output_at++) = ((sample >> 8) & 0xff);
            m_output_bytes += 2;
        }
    }
    return m_output_bytes;
}

qint64 DecoderMAD::madOutputFloat(float *data, qint64 samples)
{
    float *data_it = data;
    unsigned int samples_per_channel, channels;
    mad_fixed_t const *left, *right;

    samples_per_channel = m_synth.pcm.length;
    channels = m_synth.pcm.channels;
    left = m_synth.pcm.samples[0];
    right = m_synth.pcm.samples[1];
    m_bitrate = m_frame.header.bitrate / 1000;
    m_output_at = 0;
    m_output_bytes = 0;
    qint64 output_samples = 0;

    if(samples_per_channel * channels > samples)
    {
        qWarning("DecoderMad: input buffer is too small");
        samples_per_channel = samples / channels;
    }

    while (samples_per_channel--)
    {
        *data_it++ = mad_f_todouble(*left++);
        output_samples++;
        if (channels == 2)
        {
            *data_it++ = mad_f_todouble(*right++);
            output_samples++;
        }
    }
    return output_samples;
}
