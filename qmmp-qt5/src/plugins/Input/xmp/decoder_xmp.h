/***************************************************************************
 *   Copyright (C) 2015-2016 by Ilya Kotov                                 *
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

#ifndef DECODER_XMP_H
#define DECODER_XMP_H

#include <qmmp/decoder.h>
#include <xmp.h>

class DecoderXmp : public Decoder
{
public:
    DecoderXmp(const QString &path);
    virtual ~DecoderXmp();

    static DecoderXmp *instance();
    void readSettings();
    // Standard Decoder API
    bool initialize();
    qint64 totalTime() const;
    int bitrate() const;
    qint64 read(unsigned char *audio, qint64 maxSize);
    void seek(qint64 time);


private:
    //helper function
    void deinit();

    xmp_context m_ctx;
    qint64 m_totalTime;
    QString m_path;
    quint32 m_srate;

    static DecoderXmp *m_instance;
};

#endif // DECODER_XMP_H
