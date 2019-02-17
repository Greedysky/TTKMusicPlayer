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

#ifndef DECODER_FFAPCUE_H
#define DECODER_FFAPCUE_H

#include <qmmp/decoder.h>

class Output;
class QIDevice;
class CUEParser;

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class DecoderFFapCUE : public Decoder
{
public:
    DecoderFFapCUE(const QString &url);
    virtual ~DecoderFFapCUE();

    // Standard Decoder API
    virtual bool initialize() override;
    virtual qint64 totalTime() const override;
    virtual void seek(qint64) override;
    virtual qint64 read(unsigned char *data, qint64 size) override;
    virtual int bitrate() const override;
    virtual const QString nextURL() const override;
    virtual void next() override;

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

#endif // DECODER_FFAPCUE_H
