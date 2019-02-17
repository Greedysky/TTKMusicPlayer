/***************************************************************************
 *   Copyright (C) 2009-2019 by Ilya Kotov                                 *
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

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}
#include <stdint.h>
#include "ffmpegmetadatamodel.h"

FFmpegMetaDataModel::FFmpegMetaDataModel(const QString &path) : MetaDataModel(true)
{
    AVFormatContext *in = nullptr;
#ifdef Q_OS_WIN
    if (avformat_open_input(&in, path.toUtf8().constData(), nullptr, nullptr) < 0)
#else
    if (avformat_open_input(&in, path.toLocal8Bit().constData(), nullptr, nullptr) < 0)
#endif
        return;

    if(in)
    {
        avformat_find_stream_info(in, nullptr);
        av_read_play(in);

#if (LIBAVCODEC_VERSION_INT >= AV_VERSION_INT(57,48,0)) //ffmpeg-3.1:  57.48.101
        AVCodecParameters *c = nullptr;
#else
        AVCodecContext *c = nullptr;
#endif

        for (uint idx = 0; idx < in->nb_streams; idx++)
        {
#if (LIBAVCODEC_VERSION_INT >= AV_VERSION_INT(57,48,0)) //ffmpeg-3.1:  57.48.101
            c = in->streams[idx]->codecpar;
#else
            c = in->streams[idx]->codec;
#endif
            if (c->codec_type == AVMEDIA_TYPE_VIDEO && c->codec_id == AV_CODEC_ID_MJPEG)
                break;
        }
        if (c)
        {
            AVPacket pkt;
            av_read_frame(in, &pkt);
            m_pixmap.loadFromData(QByteArray((const char*)pkt.data, pkt.size));
        }

        avformat_close_input(&in);
    }
}

FFmpegMetaDataModel::~FFmpegMetaDataModel()
{}

QPixmap FFmpegMetaDataModel::cover() const
{
    return m_pixmap;
}
