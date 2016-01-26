/***************************************************************************
 *   Copyright (C) 2006-2009 by Ilya Kotov                                 *
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

#ifndef DECODER_MPC_H
#define DECODER_MPC_H

#ifdef MPC_OLD_API
#include <mpcdec/mpcdec.h>
#else
#include <mpc/mpcdec.h>
#endif

#include <qmmp/decoder.h>

struct mpc_data
{
#ifdef MPC_OLD_API
    mpc_decoder decoder;
#else
    mpc_demux *demuxer;
#endif
    mpc_reader reader;
    mpc_streaminfo info;
};

class DecoderMPC : public Decoder
{
public:
    DecoderMPC(QIODevice *i);
    virtual ~DecoderMPC();

    struct mpc_data *data()
    {
        return m_data;
    }

     // Standard Decoder API
    bool initialize();
    qint64 totalTime();
    int bitrate();
    qint64 read(char *audio, qint64 maxSize);
    void seek(qint64 time);

private:

    struct mpc_data *m_data;
    long m_len;
    int m_bitrate;
    qint64 m_totalTime;
};


#endif // __decoder_mpc_h
