/***************************************************************************
 *   Copyright (C) 2013-2019 by Ilya Kotov                                 *
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

#ifndef DECODER_OPUS_H
#define DECODER_OPUS_H

#include <qmmp/decoder.h>
#include <opus/opusfile.h>

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class DecoderOpus : public Decoder
{
public:
    DecoderOpus(const QString &url, QIODevice *i);
    virtual ~DecoderOpus();

    // Standard Decoder API
    virtual bool initialize() override;
    virtual qint64 totalTime() const override;
    virtual int bitrate() const override;
    virtual qint64 read(unsigned char *data, qint64 maxSize) override;
    virtual void seek(qint64 time) override;

private:
    // helper functions
    ChannelMap findChannelMap(int channels);
    OggOpusFile *m_opusfile;
    qint64 m_totalTime;
    int m_bitrate;
    int m_chan;
    QString m_url;
};

#endif // DECODER_OPUS_H
