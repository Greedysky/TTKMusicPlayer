/***************************************************************************
 *  Based on mq3 and madplay projects                                      *
 *                                                                         *
 * Copyright (c) 2000-2001 Brad Hughes <bhughes@trolltech.com>             *
 * Copyright (C) 2000-2004 Robert Leslie <rob@mars.org>                    *
 * Copyright (C) 2009-2017 Ilya Kotov forkotov02@ya.ru                     *
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
    qint64 totalTime() const;
    int bitrate() const;
    qint64 read(unsigned char *data, qint64 size);
    void seek(qint64);

private:
    //LAME header
    struct LameHeader
    {
        unsigned char revision;
        unsigned char flags;
        mad_fixed_t peak;
        unsigned short start_delay;
        unsigned short end_padding;
        qint8 gain;
    };
    // helper functions
    bool decodeFrame();
    qint64 madOutputFloat(float *data, qint64 samples);
    bool fillBuffer();
    void deinit();
    bool findHeader();
    bool findXingHeader(struct mad_bitptr, unsigned int bitlen);
    LameHeader *findLameHeader(struct mad_bitptr ptr, unsigned int bitlen);
    uint findID3v2(uchar *data, ulong size);
    bool m_inited, m_eof;
    qint64 m_totalTime;
    int m_channels, m_skip_frames;
    uint m_bitrate;
    long m_freq, m_len;

    // file input buffer
    char *m_input_buf;
    qint64 m_input_bytes;

    // MAD decoder

    //xing header
    struct XingHeader
    {
        int flags;
        unsigned long frames;
        unsigned long bytes;
        unsigned char toc[100];
        long scale;
        LameHeader *lame;

        XingHeader()
        {
            flags = 0;
            frames = 0;
            bytes = 0;
            scale = 0;
            lame = 0;
        }
    };
    XingHeader m_xing;

    enum
    {
        XING_FRAMES = 0x0001,
        XING_BYTES  = 0x0002,
        XING_TOC    = 0x0004,
        XING_SCALE  = 0x0008
    };

    struct mad_stream m_stream;
    struct mad_frame m_frame;
    struct mad_synth m_synth;
    qint64 m_skip_bytes, m_play_bytes;

};


#endif // DECODER_MAD_H
