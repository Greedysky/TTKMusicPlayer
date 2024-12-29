/***************************************************************************
 *   Copyright (C) 2011-2025 by Ilya Kotov                                 *
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

#ifndef DECODER_FFMPEGM4B_H
#define DECODER_FFMPEGM4B_H

#include <qmmp/decoder.h>

class TrackInfo;
class QIDevice;

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class DecoderFFmpegM4b : public Decoder
{
public:
    explicit DecoderFFmpegM4b(DecoderFactory *factory, const QString &path);
    virtual ~DecoderFFmpegM4b();

    // Standard Decoder API
    virtual bool initialize() override final;
    virtual qint64 totalTime() const override final;
    virtual int bitrate() const override final;
    virtual qint64 read(unsigned char *data, qint64 maxSize) override final;
    virtual void seek(qint64 time) override final;
    virtual const QString nextURL() const override final;
    virtual void next() override final;

private:
    Decoder *m_decoder = nullptr;
    char *m_buf = nullptr; //buffer for remainig data
    int m_track = 0;
    qint64 m_duration = 0;
    qint64 m_offset = 0;
    qint64 m_trackSize = 0;
    qint64 m_written = 0;
    QString m_path;
    qint64 m_bufSize = 0;
    qint64 m_frameSize = 0; //sample size
    QIODevice *m_input = nullptr;
    DecoderFactory *m_factory;

    struct ChapterInfo
    {
        TrackInfo *info;
        qint64 offset;
        qint64 duration;
        QString url;
    };

    QList<ChapterInfo> m_chapters;

};

#endif
