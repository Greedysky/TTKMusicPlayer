/***************************************************************************
 *   Copyright (C) 2008-2014 by Ilya Kotov                                 *
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

#ifndef DECODER_CUE_H
#define DECODER_CUE_H

#include <qmmp/decoder.h>
#include <qmmp/statehandler.h>

class Output;
class QIDevice;
class CUEParser;

class DecoderCUE : public Decoder
{
public:
    DecoderCUE(const QString &url);
    virtual ~DecoderCUE();

    // Standard Decoder API
    bool initialize();
    qint64 totalTime();
    void seek(qint64);
    qint64 read(char *data, qint64 size);
    int bitrate();
    const QString nextURL() const;
    void next();

private:
    Decoder *m_decoder;
    qint64 m_length;
    qint64 m_offset;
    qint64 length_in_bytes;
    qint64 m_totalBytes;
    QString m_path;
    CUEParser *m_parser;
    int m_track;
    char *m_buf; //buffer for remainig data
    qint64 m_buf_size;
    qint64 m_sz; //sample size
    QIODevice *m_input;
};

#endif // DECODER_CUE_H
