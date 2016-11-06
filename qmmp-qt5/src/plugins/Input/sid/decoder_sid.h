/***************************************************************************
 *   Copyright (C) 2013-2014 by Ilya Kotov                                 *
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

#ifndef DECODER_SID_H
#define DECODER_SID_H

#include <qmmp/decoder.h>

class sidplayfp;
class SidDatabase;

/**
   @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class DecoderSID : public Decoder
{
public:
    DecoderSID(SidDatabase *db, const QString &url);
    virtual ~DecoderSID();

    // Standard Decoder API
    bool initialize();
    qint64 totalTime();
    int bitrate();
    qint64 read(unsigned char *data, qint64 size);
    void seek(qint64);

private:
    QString m_url;
    sidplayfp *m_player;
    SidDatabase *m_db;
    int m_length;
    qint64 m_length_in_bytes;
    qint64 m_read_bytes;
};

#endif // DECODER_SID_H
