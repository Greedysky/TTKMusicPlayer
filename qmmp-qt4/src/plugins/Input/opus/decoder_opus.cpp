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

#include <QObject>
#include <QIODevice>
#include <qmmp/buffer.h>
#include <qmmp/output.h>
#include <qmmp/fileinfo.h>
#include "decoder_opus.h"

// ic functions for libopusfile
static int opusread (void *src, unsigned char *buf,int size)
{
    DecoderOpus *d = (DecoderOpus *) src;
    return d->input()->read((char *) buf, size);
}

static int opusseek(void *src, opus_int64 offset, int whence)
{
    DecoderOpus *d = (DecoderOpus *) src;
    if (d->input()->isSequential())
        return -1;
    long start = 0;
    switch (whence)
    {
    case SEEK_END:
        start = d->input()->size();
        break;

    case SEEK_CUR:
        start = d->input()->pos();
        break;

    case SEEK_SET:
    default:
        start = 0;
    }

    if (d->input()->seek(start + offset))
        return 0;
    return -1;
}

static opus_int64  opustell(void *src)
{
    DecoderOpus *dogg = (DecoderOpus *) src;
    return (long) dogg->input()->pos();
}

// Decoder class
DecoderOpus::DecoderOpus(const QString &url, QIODevice *i) : Decoder(i)
{
    m_totalTime = 0;
    m_opusfile = 0;
    m_chan = 0;
    m_bitrate = 0;
    m_url = url;
}

DecoderOpus::~DecoderOpus()
{
    if (m_opusfile)
        op_free(m_opusfile);
    m_opusfile = 0;
}

bool DecoderOpus::initialize()
{
    qDebug("DecoderOpus: initialize");
    m_chan = 0;
    m_totalTime = 0;

    if (!input())
    {
        qDebug("DecoderOpus: cannot initialize.  No input");
        return false;
    }

    if (!input()->isOpen())
    {
        if (!input()->open(QIODevice::ReadOnly))
        {
            qWarning("DecoderOpus: unable to open input. Error: %s",qPrintable(input()->errorString()));
            return false;
        }
    }

    OpusFileCallbacks opuscb =
    {
        opusread,
        opusseek,
        opustell,
        0,
    };
    m_opusfile = op_open_callbacks(this, &opuscb, 0, 0, 0);

    if (!m_opusfile)
    {
        qWarning("DecoderOpus: cannot open stream");
        return false;
    }

    m_bitrate = op_bitrate(m_opusfile, -1) / 1000;

    if((m_totalTime = op_pcm_total(m_opusfile, -1) / 48) < 0)
        m_totalTime = 0;

    const OpusHead *head = op_head(m_opusfile, -1);
    if (!head)
    {
        qWarning("DecoderOpus: unable to read header");
        return false;
    }

    m_chan =  head->channel_count;

    ChannelMap chmap = findChannelMap(m_chan);
    if(chmap.isEmpty())
    {
        qWarning("DecoderOpus: unsupported number of channels: %d", m_chan);
        return false;
    }
    configure(48000, chmap, Qmmp::PCM_S16LE); //opus codec supports 48 kHz only
    return true;
}

qint64 DecoderOpus::totalTime()
{
    if (!m_opusfile)
        return 0;
    return m_totalTime;
}

int DecoderOpus::bitrate()
{
    return m_bitrate;
}

void DecoderOpus::seek(qint64 time)
{
    op_pcm_seek(m_opusfile, time*48);
}

qint64 DecoderOpus::read(char *data, qint64 maxSize)
{
    int samples = maxSize / 2 / m_chan;
    samples = op_read(m_opusfile, (opus_int16 *)data, samples, 0);
    m_bitrate = op_bitrate_instant(m_opusfile) / 1000;
    return samples * m_chan * 2;
}

//https://tools.ietf.org/id/draft-ietf-codec-oggopus-04.txt
ChannelMap DecoderOpus::findChannelMap(int channels)
{
    ChannelMap map;
    switch (channels)
    {
    case 1:
        map << Qmmp::CHAN_FRONT_LEFT;
        break;
    case 2:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_RIGHT;
        break;
    case 3:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_CENTER
            << Qmmp::CHAN_FRONT_RIGHT;
        break;
    case 4:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_RIGHT
            << Qmmp::CHAN_REAR_LEFT
            << Qmmp::CHAN_REAR_RIGHT;
        break;
    case 5:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_CENTER
            << Qmmp::CHAN_FRONT_RIGHT
            << Qmmp::CHAN_REAR_LEFT
            << Qmmp::CHAN_REAR_RIGHT;
        break;
    case 6:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_CENTER
            << Qmmp::CHAN_FRONT_RIGHT
            << Qmmp::CHAN_REAR_LEFT
            << Qmmp::CHAN_REAR_RIGHT
            << Qmmp::CHAN_LFE;
        break;
    case 7:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_CENTER
            << Qmmp::CHAN_FRONT_RIGHT
            << Qmmp::CHAN_SIDE_LEFT
            << Qmmp::CHAN_SIDE_RIGHT
            << Qmmp::CHAN_REAR_CENTER
            << Qmmp::CHAN_LFE;
        break;
    case 8:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_CENTER
            << Qmmp::CHAN_FRONT_RIGHT
            << Qmmp::CHAN_SIDE_LEFT
            << Qmmp::CHAN_SIDE_RIGHT
            << Qmmp::CHAN_REAR_LEFT
            << Qmmp::CHAN_REAR_RIGHT
            << Qmmp::CHAN_LFE;
        break;
    default:
        ;
    }
    return map;
}
