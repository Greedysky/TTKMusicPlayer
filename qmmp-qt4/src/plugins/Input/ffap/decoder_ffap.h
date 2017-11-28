/***************************************************************************
 *   Copyright (C) 2011-2016 by Ilya Kotov                                 *
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

#ifndef DECODER_FFAP_H
#define DECODER_FFAP_H

#include <qmmp/decoder.h>
#include "ffap.h"

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class DecoderFFap : public Decoder
{
public:
    DecoderFFap(const QString &path, QIODevice *i);
    virtual ~DecoderFFap();

    // Standard Decoder API
    bool initialize();
    qint64 totalTime() const;
    int bitrate() const;
    qint64 read(unsigned char *data, qint64 maxSize);
    void seek(qint64 time);

private:
    // helper functions
    void deinit();
    //uint findID3v2(char *data, ulong size); //retuns ID3v2 tag size
    FFap_decoder *m_ffap_decoder;
    QString m_path;
};


#endif // DECODER_FFAP_H
