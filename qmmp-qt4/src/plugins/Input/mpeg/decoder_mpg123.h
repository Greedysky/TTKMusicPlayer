/***************************************************************************
 *   Copyright (C) 2011-2019 by Ilya Kotov                                 *
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

#ifndef DECODER_MPG123_H
#define DECODER_MPG123_H

class QIODevice;

#include <mpg123.h>
#include <qmmp/decoder.h>
#include "decodermpegfactory.h"

class DecoderMPG123 : public Decoder
{
public:
    DecoderMPG123(QIODevice *i);
    virtual ~DecoderMPG123();

    // standard decoder API
    bool initialize();
    qint64 totalTime() const;
    int bitrate() const;
    qint64 read(unsigned char *data, qint64 size);
    void seek(qint64);

private:
    void cleanup(mpg123_handle *handle);
    void setMPG123Format(int encoding);
    mpg123_handle *m_handle;
    mpg123_frameinfo m_frame_info;
    qint64 m_totalTime;
    long m_rate;
    int m_mpg123_encoding;
    int m_errors;
};

#endif // DECODER_MPG123_H
