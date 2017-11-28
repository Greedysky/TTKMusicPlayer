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

#include <QObject>
#include <QSettings>
#include <QDir>
#include <QSettings>
#include "decoder_xmp.h"

// Decoder class

DecoderXmp *DecoderXmp::m_instance = 0;

DecoderXmp::DecoderXmp(const QString &path) : Decoder(0)
{
    m_instance = this;
    m_path = path;
    m_ctx = 0;
    m_srate = 44100;
    m_totalTime = 0;
}

DecoderXmp::~DecoderXmp()
{
    if(m_instance == this)
        m_instance = 0;
    deinit();
}

DecoderXmp *DecoderXmp::instance()
{
    return m_instance;
}

bool DecoderXmp::initialize()
{
    m_ctx = xmp_create_context();

    int err = xmp_load_module(m_ctx, m_path.toLocal8Bit().data());
    if(err != 0)
    {
        qWarning("DecoderXmp: unable to load module file, error = %d", err);
        xmp_free_context(m_ctx);
        m_ctx = 0;
        return false;
    }

    xmp_module_info mi;
    xmp_get_module_info(m_ctx, &mi);

    m_totalTime = mi.seq_data[0].duration;

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_srate = settings.value("Xmp/sample_rate", 44100).toInt();

    xmp_start_player(m_ctx, m_srate, 0);
    readSettings();

    configure(m_srate, 2, Qmmp::PCM_S16LE);
    return true;
}

qint64 DecoderXmp::totalTime() const
{
    return m_totalTime;
}

int DecoderXmp::bitrate() const
{
    return 8;
}

qint64 DecoderXmp::read(unsigned char *audio, qint64 maxSize)
{
    int c = xmp_play_buffer(m_ctx, audio, maxSize, 1);

    if(c == 0)
        return maxSize;
    else if(c == -XMP_END)
        return 0;

    return -1;
}

void DecoderXmp::seek(qint64 pos)
{
    xmp_seek_time(m_ctx, pos);
}

void DecoderXmp::deinit()
{
    if(m_ctx)
    {
        xmp_end_player(m_ctx);
        xmp_release_module(m_ctx);
        xmp_free_context(m_ctx);
        m_ctx = 0;
    }
}

void DecoderXmp::readSettings()
{
    if(m_ctx)
    {
        QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
        settings.beginGroup("Xmp");
        xmp_set_player(m_ctx, XMP_PLAYER_AMP, settings.value("amp_factor", 1).toInt());
        xmp_set_player(m_ctx, XMP_PLAYER_MIX, settings.value("stereo_mix", 70).toInt());
        xmp_set_player(m_ctx, XMP_PLAYER_INTERP, settings.value("interpolation", XMP_INTERP_LINEAR).toInt());
        int flags = 0;
        if(settings.value("lowpass", false).toBool())
            flags |= XMP_DSP_LOWPASS;
        xmp_set_player(m_ctx, XMP_PLAYER_DSP, flags);
        flags = 0;
        if(settings.value("vblank", false).toBool())
            flags |= XMP_FLAGS_VBLANK;
        if(settings.value("fx9bug", false).toBool())
            flags |= XMP_FLAGS_FX9BUG;
        xmp_set_player(m_ctx, XMP_PLAYER_FLAGS, flags);


        settings.endGroup();
    }
}
