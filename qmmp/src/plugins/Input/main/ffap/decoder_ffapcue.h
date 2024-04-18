/***************************************************************************
 *   Copyright (C) 2011-2024 by Ilya Kotov                                 *
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

#include <taglib/apetag.h>
#include <taglib/apefile.h>
#include <taglib/tfilestream.h>
#include <qmmp/decoder.h>
#include <qmmp/cueparser.h>

class Output;
class QIDevice;

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class DecoderFFapCUE : public Decoder
{
public:
    explicit DecoderFFapCUE(const QString &path);
    virtual ~DecoderFFapCUE();

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
    CueParser *m_parser = nullptr;
    char *m_buf = nullptr; //buffer for remainig data
    qint64 m_duration = 0;
    qint64 m_offset = 0;
    qint64 m_trackSize = 0;
    qint64 m_written = 0;
    QString m_path;
    int m_track = 0;
    qint64 m_buf_size = 0;
    qint64 m_frameSize = 0;
    QIODevice *m_input = nullptr;

};

#endif
