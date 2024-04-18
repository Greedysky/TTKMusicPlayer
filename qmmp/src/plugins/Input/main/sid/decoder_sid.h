/***************************************************************************
 *   Copyright (C) 2013-2024 by Ilya Kotov                                 *
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

#ifndef DECODER_SID_H
#define DECODER_SID_H

#include <qmmp/decoder.h>

class sidplayfp;
class SidDatabase;
class SidTune;

/**
   @author Ilya Kotov <forkotov02@ya.ru>
*/
class DecoderSID : public Decoder
{
public:
    DecoderSID(SidDatabase *db, const QString &path);
    virtual ~DecoderSID();

    // Standard Decoder API
    virtual bool initialize() override final;
    virtual qint64 totalTime() const override final;
    virtual int bitrate() const override final;
    virtual qint64 read(unsigned char *data, qint64 maxSize) override final;
    virtual void seek(qint64 time) override final;

private:
    QString m_path;
    sidplayfp *m_player;
    SidDatabase *m_db;
    qint64 m_length = 0;
    qint64 m_length_in_bytes = 0;
    qint64 m_read_bytes = 0;
    SidTune m_tune;

};

#endif
