/***************************************************************************
 *   Copyright (C) 2006-2013 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
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

#ifndef __decoder_ffmeg_h
#define __decoder_ffmeg_h

extern "C"{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/mathematics.h>
#if (LIBAVUTIL_VERSION_INT >= ((51<<16)+(32<<8)+0))
#include <libavutil/dict.h>
#endif
}


#include <qmmp/decoder.h>

#define PROBE_BUFFER_SIZE 8192
#define INPUT_BUFFER_SIZE 16384

class DecoderFFmpeg : public Decoder
{
public:
    DecoderFFmpeg(const QString &, QIODevice *i);
    virtual ~DecoderFFmpeg();

    // Standard Decoder API
    bool initialize();
    qint64 totalTime();
    int bitrate();
    qint64 read(char *audio, qint64 maxSize);
    void seek(qint64 time);

private:
    //helper functions
    void fillBuffer();

    AVFormatContext *ic;
    AVCodecContext *c;

    uint wma_st_buff, wma_idx2;
    int m_bitrate, wma_idx;

    QString m_path;
    qint64 m_totalTime;
    AVPacket m_pkt;
    AVPacket m_temp_pkt;
    qint64 m_output_at;
    uchar m_input_buf[INPUT_BUFFER_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
    int64_t m_seekTime;
    qint64  m_skipBytes;

    qint64 ffmpeg_decode(uint8_t *audio);
    bool m_skip;

#if (LIBAVCODEC_VERSION_INT >= ((52<<16)+(102<<8)+0))
    AVIOContext *m_stream;
#else
    ByteIOContext *m_stream;
#endif

    uint8_t *m_output_buf;
};


#endif // __decoder_ffmpeg_h
