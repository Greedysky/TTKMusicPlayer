/***************************************************************************
 *   Copyright (C) 2006-2012 by Ilya Kotov                                 *
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


#include <QObject>
#include <QIODevice>
#include <qmmp/buffer.h>
#include <qmmp/output.h>
#include <math.h>
#include "decoder_mpc.h"

// this function used from xmms
inline static void copyBuffer(MPC_SAMPLE_FORMAT* pInBuf, char* pOutBuf, unsigned pLength)
{
    unsigned pSize = 16;
    int clipMin    = -1 << (pSize - 1);
    int clipMax    = (1 << (pSize - 1)) - 1;
    int floatScale =  1 << (pSize - 1);
    for (unsigned n = 0; n < 2 * pLength; n++)
    {
        int val;
#ifdef MPC_FIXED_POINT
        val = shiftSigned(pInBuf[n], pSize - MPC_FIXED_POINT_SCALE_SHIFT);
#else
        val = (int) (pInBuf[n] * floatScale);
#endif
        if (val < clipMin)
            val = clipMin;
        else if (val > clipMax)
            val = clipMax;
        unsigned shift = 0;
        do
        {
            pOutBuf[n * 2 + (shift / 8)] = (unsigned char) ((val >> shift) & 0xFF);
            shift += 8;
        }
        while (shift < pSize);
    }
}

// mpc callbacks

#ifdef MPC_OLD_API
static mpc_int32_t mpc_callback_read (void *data, void *buffer, mpc_int32_t size)
{
    DecoderMPC *dmpc = (DecoderMPC *) data;
#else
static mpc_int32_t mpc_callback_read (mpc_reader *reader, void *buffer, mpc_int32_t size)
{
    DecoderMPC *dmpc = (DecoderMPC *) reader->data;
#endif
    qint64 res;

    res = dmpc->input()->read((char *)buffer, size);

    return res;
}
#ifdef MPC_OLD_API
static mpc_bool_t mpc_callback_seek (void *data, mpc_int32_t offset)
{
    DecoderMPC *dmpc = (DecoderMPC *) data;
#else
static mpc_bool_t mpc_callback_seek (mpc_reader *reader, mpc_int32_t offset)
{
    DecoderMPC *dmpc = (DecoderMPC *) reader->data;
#endif
    return dmpc->input()->seek(offset);
}
#ifdef MPC_OLD_API
static mpc_int32_t mpc_callback_tell (void *data)
{
    DecoderMPC *dmpc = (DecoderMPC *) data;
#else
static mpc_int32_t mpc_callback_tell (mpc_reader *reader)
{
    DecoderMPC *dmpc = (DecoderMPC *) reader->data;
#endif
    return dmpc->input()->pos ();
}
#ifdef MPC_OLD_API
static mpc_bool_t  mpc_callback_canseek (void *data)
{
    DecoderMPC *dmpc = (DecoderMPC *) data;
#else
static mpc_bool_t  mpc_callback_canseek (mpc_reader *reader)
{
    DecoderMPC *dmpc = (DecoderMPC *) reader->data;
#endif
    return !dmpc->input()->isSequential () ;
}
#ifdef MPC_OLD_API
static mpc_int32_t mpc_callback_get_size (void *data)
{
    DecoderMPC *dmpc = (DecoderMPC *) data;
#else
static mpc_int32_t mpc_callback_get_size (mpc_reader *reader)
{
    DecoderMPC *dmpc = (DecoderMPC *) reader->data;
#endif
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
    if (data())
    {
#ifndef MPC_OLD_API
        if (data()->demuxer)
            mpc_demux_exit (data()->demuxer);
        data()->demuxer = 0;
#endif
        delete data();
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

    if (!input()->isOpen())
    {
        if (!input()->open(QIODevice::ReadOnly))
        {
            qWarning("DecoderMPC: unable to open input.");
            return false;
        }
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

#ifdef MPC_OLD_API
    mpc_streaminfo_init (&m_data->info);
    if (mpc_streaminfo_read (&m_data->info, &m_data->reader) != ERROR_CODE_OK)
        return false;
#else
    m_data->demuxer = mpc_demux_init (&m_data->reader);

    if (!m_data->demuxer)
        return false;
    mpc_demux_get_info (m_data->demuxer, &m_data->info);
#endif

    int chan = data()->info.channels;
    configure(data()->info.sample_freq, chan, Qmmp::PCM_S16LE);
    QMap<Qmmp::ReplayGainKey, double> rg_info; //replay gain information
#ifdef MPC_OLD_API
    mpc_decoder_setup (&data()->decoder, &data()->reader);

    //mpc_decoder_scale_output (&data()->decoder, 3.0);
    if (!mpc_decoder_initialize (&data()->decoder, &data()->info))
    {
        qWarning("DecoderMPC: cannot get info.");
        return false;
    }
    rg_info[Qmmp::REPLAYGAIN_ALBUM_GAIN] = data()->info.gain_album/100.0;
    rg_info[Qmmp::REPLAYGAIN_TRACK_GAIN] = data()->info.gain_title/100.0;
    rg_info[Qmmp::REPLAYGAIN_ALBUM_PEAK] = data()->info.peak_album/32768.0;
    rg_info[Qmmp::REPLAYGAIN_TRACK_PEAK] = data()->info.peak_title/32768.0;
#else
    rg_info[Qmmp::REPLAYGAIN_ALBUM_GAIN] = data()->info.gain_album/256.0;
    rg_info[Qmmp::REPLAYGAIN_TRACK_GAIN] = data()->info.gain_title/256.0;
    rg_info[Qmmp::REPLAYGAIN_ALBUM_PEAK] = pow(10, data()->info.peak_album/256.0/20.0);
    rg_info[Qmmp::REPLAYGAIN_TRACK_PEAK] = pow(10, data()->info.peak_title/256.0/20.0);
#endif
    setReplayGainInfo(rg_info);

    m_totalTime = mpc_streaminfo_get_length(&data()->info) * 1000;
    qDebug("DecoderMPC: initialize succes");
    return true;
}

qint64 DecoderMPC::totalTime()
{
    return m_totalTime;
}

int DecoderMPC::bitrate()
{
    return m_bitrate;
}

qint64 DecoderMPC::read(char *audio, qint64 maxSize)
{
#ifdef MPC_OLD_API
    mpc_uint32_t vbrAcc = 0;
    mpc_uint32_t vbrUpd = 0;
    MPC_SAMPLE_FORMAT buffer[MPC_DECODER_BUFFER_LENGTH];
    m_len = mpc_decoder_decode (&data()->decoder, buffer, &vbrAcc, &vbrUpd);
    copyBuffer(buffer, audio, qMin(m_len,long(maxSize/4)));
    m_len = m_len * 4;
    m_bitrate = vbrUpd * data()->info.sample_freq / 1152000;
#else
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
            m_len = frame.samples;
            copyBuffer(buffer, audio, qMin(m_len,long(maxSize/4)));
            m_len = m_len * 4;
        }
    }
    m_bitrate = frame.bits * data()->info.sample_freq / 1152000;
#endif
    return m_len;
}

void DecoderMPC::seek(qint64 pos)
{
#ifdef MPC_OLD_API
    mpc_decoder_seek_seconds(&data()->decoder, pos/1000);
#else
    mpc_demux_seek_second(data()->demuxer, (double)pos/1000);
#endif
}
