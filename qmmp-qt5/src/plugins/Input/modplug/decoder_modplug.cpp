/***************************************************************************
 *   Copyright (C) 2008-2015 by Ilya Kotov                                 *
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

/* Based on Modplug XMMS Plugin
 * Authors: Kenton Varda <temporal@gauge3d.org>
 */

#include <QObject>
#include <QIODevice>
#include <QFile>
#include <QDir>
#include <QSettings>
#include <math.h>
#include <libmodplug/stdafx.h>
#include <libmodplug/it_defs.h>
#include <libmodplug/sndfile.h>
#include <qmmp/buffer.h>
#include <qmmp/output.h>
#include <stdint.h>
#include "archivereader.h"
#include "decoder_modplug.h"

// Decoder class

DecoderModPlug* DecoderModPlug::m_instance = 0;

DecoderModPlug::DecoderModPlug(const QString &path) : Decoder(0)
{
    m_path = path;
    m_freq = 0;
    m_bitrate = 0;
    m_totalTime = 0;
    m_chan = 0;
    m_soundFile = 0;
    m_sampleSize = 0;
    m_instance = this;
}

DecoderModPlug::~DecoderModPlug()
{
    deinit();
    if(m_instance == this)
        m_instance = 0;
}

bool DecoderModPlug::initialize()
{
    m_freq = m_bitrate = 0;
    m_chan = 0;
    m_totalTime = 0;

    ArchiveReader reader;
    if (reader.isSupported(m_path))
        m_input_buf = reader.unpack(m_path);
    else
    {
        QFile file(m_path);
        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning("DecoderModPlug: error: %s", qPrintable(file.errorString ()));
            return false;
        }
        m_input_buf = file.readAll();
        file.close();
    }
    if (m_input_buf.isEmpty())
    {
        qWarning("DecoderModPlug: error while reading module file");
        return false;
    }
    m_soundFile = new CSoundFile();
    readSettings();
    m_sampleSize = m_bps / 8 * m_chan;
    m_soundFile->Create((uchar*) m_input_buf.data(), m_input_buf.size());
    m_bitrate = m_soundFile->GetNumChannels();
    m_totalTime = (qint64) m_soundFile->GetSongTime() * 1000;
    configure(m_freq, m_chan, (m_bps == 8 ? Qmmp::PCM_S8 : Qmmp::PCM_S16LE));
    return true;
}

qint64 DecoderModPlug::totalTime()
{
    return m_totalTime;
}

int DecoderModPlug::bitrate()
{
    return m_bitrate;
}

qint64 DecoderModPlug::read(unsigned char *audio, qint64 maxSize)
{
    long len = m_soundFile->Read (audio, maxSize) * m_sampleSize;
    if (m_usePreamp)
    {
        {
            //apply preamp
            if (m_bps == 16)
            {
                long n = len >> 1;
                for (long i = 0; i < n; i++)
                {
                    short old = ((short*)audio)[i];
                    ((short*)audio)[i] *= m_preampFactor;
                    // detect overflow and clip!
                    if ((old & 0x8000) !=
                            (((short*)audio)[i] & 0x8000))
                        ((short*)audio)[i] = old | 0x7FFF;
                }
            }
            else
            {
                for (long i = 0; i < len; i++)
                {
                    uchar old = ((uchar*)audio)[i];
                    ((uchar*)audio)[i] *= m_preampFactor;
                    // detect overflow and clip!
                    if ((old & 0x80) !=
                            (((uchar*)audio)[i] & 0x80))
                        ((uchar*)audio)[i] = old | 0x7F;
                }
            }
        }
    }
    return len;
}

void DecoderModPlug::seek(qint64 pos)
{
    quint32 lMax;
    quint32 lMaxtime;
    double lPostime;

    if (pos > (lMaxtime = m_soundFile->GetSongTime()) * 1000)
        pos = lMaxtime * 1000;
    lMax = m_soundFile->GetMaxPosition();
    lPostime = float(lMax) / lMaxtime;
    m_soundFile->SetCurrentPos(int(pos * lPostime / 1000));
}

void DecoderModPlug::deinit()
{
    m_freq = m_bitrate = 0;
    m_chan = 0;
    if (m_soundFile)
    {
        m_soundFile->Destroy();
        delete m_soundFile;
        m_soundFile = 0;
    }
    m_input_buf.clear();
}

void DecoderModPlug::readSettings()
{
    if (!m_soundFile)
        return;
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("ModPlug");
    CSoundFile::SetWaveConfig
    (
        m_freq = settings.value("Frequency", 44100).toInt(),
        m_bps = settings.value("Bits", 16).toInt(),
        m_chan = settings.value("Channels", 2).toInt()
    );

    CSoundFile::SetWaveConfigEx
    (
        settings.value("Surround", true).toBool(),
        true,
        settings.value("Reverb", false).toBool(),
        true,
        settings.value("Megabass", false).toBool(),
        settings.value("NoiseReduction", false).toBool(),
        false
    );
    if (settings.value("Reverb", false).toBool())
    {
        CSoundFile::SetReverbParameters
        (
            settings.value("ReverbDepth", 30).toInt(),
            settings.value("ReverbDelay", 100).toInt()
        );
    }
    if (settings.value("Megabass", false).toBool())
    {
        CSoundFile::SetXBassParameters
        (
            settings.value("BassAmount", 40).toInt(),
            settings.value("BassRange", 30).toInt()
        );
    }
    if (settings.value("Surround", true).toBool())
    {
        CSoundFile::SetSurroundParameters
        (
            settings.value("SurroundDepth", 20).toInt(),
            settings.value("SurroundDelay", 20).toInt()
        );
    }
    CSoundFile::SetResamplingMode(settings.value("ResamplineMode", SRCMODE_POLYPHASE).toInt());
    m_soundFile->SetRepeatCount(settings.value("LoopCount", 0).toInt());


    //general
    /*
     settings.value("GrabAmigaMOD", true).toBool());*/
    //preamp
    m_usePreamp = settings.value("PreAmp", false).toBool();
    m_preampFactor = exp(settings.value("PreAmpLevel", 0.0f).toDouble());
    settings.endGroup();
}

DecoderModPlug* DecoderModPlug::instance()
{
    return m_instance;
}
