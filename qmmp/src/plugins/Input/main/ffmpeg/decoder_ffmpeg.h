/***************************************************************************
 *   Copyright (C) 2006-2025 by Ilya Kotov                                 *
 *   forkotov02@ya.ru                                                      *
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

#ifndef DECODER_FFMPEG_H
#define DECODER_FFMPEG_H

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/mathematics.h>
#include <libavutil/dict.h>
}

#include <QHash>
#include <qmmp/decoder.h>

#define PROBE_BUFFER_SIZE 8192
#define INPUT_BUFFER_SIZE 16384

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class DecoderFFmpeg : public Decoder
{
public:
    DecoderFFmpeg(const QString &path, QIODevice *input);
    virtual ~DecoderFFmpeg();

    // Standard Decoder API
    virtual bool initialize() override final;
    virtual qint64 totalTime() const override final;
    virtual int bitrate() const override final;
    virtual qint64 read(unsigned char *data, qint64 maxSize) override final;
    virtual void seek(qint64 time) override final;

private:
    //helper functions
    void fillBuffer();

    AVFormatContext *m_formatContext = nullptr;
    AVCodecContext *m_codecContext = nullptr;
    AVIOContext *m_stream = nullptr;
    AVFrame *m_frame = nullptr;
    int m_bitrate = 0, m_audioIndex = 0;

    QString m_path;
    qint64 m_totalTime = 0;
    AVPacket *m_pkt = nullptr;
    qint64 m_output_size = 0;
    uchar *m_input_buf = nullptr;
    int64_t m_seekTime = -1;
    qint64  m_skipBytes = 0;
    int m_channels = 0;
    bool m_eof = false;

#if LIBAVUTIL_VERSION_INT >= AV_VERSION_INT(57, 28, 100) //ffmpeg-5.1
    static const QHash<AVChannel, Qmmp::ChannelPosition> m_ff_channels;
#endif
};

#endif
