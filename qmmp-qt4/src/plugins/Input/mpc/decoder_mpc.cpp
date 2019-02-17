/***************************************************************************
 *   Copyright (C) 2006-2019 by Ilya Kotov                                 *
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

#include <QObject>
#include <QIODevice>
#include <qmmp/buffer.h>
#include <qmmp/output.h>
#include <math.h>
#include "decoder_mpc.h"

// mpc callbacks

static mpc_int32_t mpc_callback_read (mpc_reader *reader, void *buffer, mpc_int32_t size)
{
    DecoderMPC *dmpc = (DecoderMPC *) reader->data;
    return dmpc->input()->read((char *)buffer, size);
}

static mpc_bool_t mpc_callback_seek (mpc_reader *reader, mpc_int32_t offset)
{
    DecoderMPC *dmpc = (DecoderMPC *) reader->data;
    return dmpc->input()->seek(offset);
}

static mpc_int32_t mpc_callback_tell (mpc_reader *reader)
{
    DecoderMPC *dmpc = (DecoderMPC *) reader->data;
    return dmpc->input()->pos ();
}

static mpc_bool_t  mpc_callback_canseek (mpc_reader *reader)
{
    DecoderMPC *dmpc = (DecoderMPC *) reader->data;
    return !dmpc->input()->isSequential () ;
}

static mpc_int32_t mpc_callback_get_size (mpc_reader *reader)
{
    DecoderMPC *dmpc = (DecoderMPC *) reader->data;
    return dmpc->input()->size();
}

// Decoder class

DecoderMPC::DecoderMPC(QIODevice *i)
        : Decoder(i)
{
    m_len = 0;
    m_bitrate = 0;
    m_totalTime = 0.0;
    m_data = 0;
}

DecoderMPC::~DecoderMPC()
{
    m_len = 0;
    if(m_data)
    {
        if(m_data->demuxer)
            mpc_demux_exit(m_data->demuxer);
        delete m_data;
        m_data = 0;
    }
}

bool DecoderMPC::initialize()
{
    m_bitrate = 0;
    m_totalTime = 0;

    if (!input())
    {
        qWarning("DecoderMPC: cannot initialize.  No input.");
        return false;
    }

    if (!m_data)
    {
        m_data = new mpc_data;
    }

    qDebug("DecoderMPC: setting callbacks");
    m_data->reader.read = mpc_callback_read;
    m_data->reader.seek = mpc_callback_seek;
    m_data->reader.tell = mpc_callback_tell;
    m_data->reader.canseek = mpc_callback_canseek;
    m_data->reader.get_size = mpc_callback_get_size;
    m_data->reader.data = this;

    m_data->demuxer = mpc_demux_init (&m_data->reader);

    if (!m_data->demuxer)
        return false;
    mpc_demux_get_info (m_data->demuxer, &m_data->info);

    int chan = data()->info.channels;
    configure(data()->info.sample_freq, chan, Qmmp::PCM_FLOAT);
    QMap<Qmmp::ReplayGainKey, double> rg_info; //replay gain information
    rg_info[Qmmp::REPLAYGAIN_ALBUM_GAIN] = data()->info.gain_album/256.0;
    rg_info[Qmmp::REPLAYGAIN_TRACK_GAIN] = data()->info.gain_title/256.0;
    rg_info[Qmmp::REPLAYGAIN_ALBUM_PEAK] = pow(10, data()->info.peak_album/256.0/20.0);
    rg_info[Qmmp::REPLAYGAIN_TRACK_PEAK] = pow(10, data()->info.peak_title/256.0/20.0);
    setReplayGainInfo(rg_info);

    m_totalTime = mpc_streaminfo_get_length(&data()->info) * 1000;
    qDebug("DecoderMPC: initialize succes");
    return true;
}

qint64 DecoderMPC::totalTime() const
{
    return m_totalTime;
}

int DecoderMPC::bitrate() const
{
    return m_bitrate;
}

qint64 DecoderMPC::read(unsigned char *audio, qint64 maxSize)
{
    mpc_frame_info frame;
    mpc_status err;
    MPC_SAMPLE_FORMAT buffer[MPC_DECODER_BUFFER_LENGTH];
    frame.buffer = (MPC_SAMPLE_FORMAT *) &buffer;
    m_len = 0;
    while (!m_len)
    {
        err = mpc_demux_decode (m_data->demuxer, &frame);
        if (err != MPC_STATUS_OK || frame.bits == -1)
        {
            m_len = 0;
            qDebug("finished");
            return 0;
        }
        else
        {
            m_len = frame.samples * data()->info.channels;
            memcpy(audio, buffer, qMin(qint64(m_len * sizeof(float)), maxSize));
        }
    }
    m_bitrate = frame.bits * data()->info.sample_freq / 1152000;
    return m_len * sizeof(float);
}

void DecoderMPC::seek(qint64 pos)
{
    mpc_demux_seek_second(data()->demuxer, (double)pos/1000);
}
