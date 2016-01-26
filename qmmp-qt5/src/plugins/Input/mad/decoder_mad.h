/***************************************************************************
 *  Based on mq3 and madplay projects                                      *
 *                                                                         *
 * Copyright (c) 2000-2001 Brad Hughes <bhughes@trolltech.com>             *
 * Copyright (C) 2000-2004 Robert Leslie <rob@mars.org>                    *
 * Copyright (C) 2009-2012 Ilya Kotov forkotov02@hotmail.ru                *
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

#ifndef   DECODER_MAD_H
#define   DECODER_MAD_H

class QIODevice;
class DecoderMAD;

#include <qmmp/decoder.h>
#include "decodermadfactory.h"

extern "C"
{
#include <mad.h>
}

class DecoderMAD : public Decoder
{
public:
    DecoderMAD(QIODevice *i);
    virtual ~DecoderMAD();

    // standard decoder API
    bool initialize();
    qint64 totalTime();
    int bitrate();
    qint64 read(char *data, qint64 size);
    qint64 read(float *data, qint64 samples);
    void seek(qint64);

private:
    // helper functions
    bool decodeFrame();
    qint64 madOutput(char *data, qint64 size);
    qint64 madOutputFloat(float *data, qint64 samples);
    bool fillBuffer();
    void deinit();
    bool findHeader();
    bool findXingHeader(struct mad_bitptr, unsigned int);
    uint findID3v2(uchar *data, ulong size);
    bool m_inited, m_eof;
    qint64 m_totalTime;
    int m_channels, m_skip_frames;
    uint m_bitrate;
    long m_freq, m_len;
    qint64 m_output_bytes, m_output_at;

    // file input buffer
    char *m_input_buf;
    qint64 m_input_bytes;

    // MAD decoder
    struct
    {
        int flags;
        unsigned long frames;
        unsigned long bytes;
        unsigned char toc[100];
        long scale;
    } xing;

    enum
    {
        XING_FRAMES = 0x0001,
        XING_BYTES  = 0x0002,
        XING_TOC    = 0x0004,
        XING_SCALE  = 0x0008
    };

    struct audio_dither
    {
        mad_fixed_t error[3];
        mad_fixed_t random;
    };

    struct mad_stream m_stream;
    struct mad_frame m_frame;
    struct mad_synth m_synth;
    struct audio_dither m_left_dither, m_right_dither;

    enum
    {
        CLIP_MIN = -MAD_F_ONE,
        CLIP_MAX =  MAD_F_ONE - 1
    };

    //converter functions
    unsigned long prng(unsigned long state);
    long audio_linear_dither(unsigned int bits, mad_fixed_t sample, struct audio_dither *dither);
    long audio_linear_round(unsigned int bits, mad_fixed_t sample);
};


#endif // DECODER_MAD_H
