/***************************************************************************
 *   Copyright (C) 2007-2016 by Ilya Kotov                                 *
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
#include <QFile>
#include <QFileInfo>
#include <qmmp/buffer.h>
#include <qmmp/output.h>
#include "decoder_sndfile.h"

// Decoder class

DecoderSndFile::DecoderSndFile(const QString &path)
        : Decoder()
{
    m_path = path;
    m_bitrate = 0;
    m_totalTime = 0;
    m_sndfile = 0;
    m_freq = 0;
}

DecoderSndFile::~DecoderSndFile()
{
    deinit();
}

bool DecoderSndFile::initialize()
{
    m_bitrate = 0;
    m_totalTime = 0.0;
    SF_INFO snd_info;

    memset (&snd_info, 0, sizeof(snd_info));
    snd_info.format=0;
#ifdef Q_OS_WIN
    m_sndfile = sf_wchar_open(reinterpret_cast<LPCWSTR>(m_path.utf16()), SFM_READ, &snd_info);
#else
    m_sndfile = sf_open(m_path.toLocal8Bit().constData(), SFM_READ, &snd_info);
#endif
    if (!m_sndfile)
    {
        qWarning("DecoderSndFile: failed to open: %s", qPrintable(m_path));
        return false;
    }

    m_freq = snd_info.samplerate;
    int chan = snd_info.channels;
    m_totalTime = snd_info.frames * 1000 / m_freq;
    m_bitrate =  QFileInfo(m_path).size () * 8.0 / m_totalTime + 0.5;

    if((snd_info.format & SF_FORMAT_SUBMASK) == SF_FORMAT_FLOAT)
        sf_command (m_sndfile, SFC_SET_SCALE_FLOAT_INT_READ, NULL, SF_TRUE);

    configure(m_freq, chan, Qmmp::PCM_S16LE);
    qDebug("DecoderSndFile: detected format: %08X", snd_info.format);
    qDebug("DecoderSndFile: initialize succes");
    return true;
}

void DecoderSndFile::deinit()
{
    m_totalTime = 0;
    m_bitrate = 0;
    m_freq = 0;
    if (m_sndfile)
        sf_close(m_sndfile);
    m_sndfile = 0;
}

qint64 DecoderSndFile::totalTime()
{
    return m_totalTime;
}

int DecoderSndFile::bitrate()
{
    return m_bitrate;
}

qint64 DecoderSndFile::read(unsigned char *audio, qint64 maxSize)
{
    return sizeof(short)* sf_read_short  (m_sndfile, (short *)audio, maxSize / sizeof(short));
}

void DecoderSndFile::seek(qint64 pos)
{
    sf_seek(m_sndfile, m_freq * pos/1000, SEEK_SET);
}
