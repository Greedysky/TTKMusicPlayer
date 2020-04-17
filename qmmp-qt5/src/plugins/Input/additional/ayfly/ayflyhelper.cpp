/* =================================================
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2020 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include "ayflyhelper.h"
#include <QDebug>
AyflyHelper::AyflyHelper(const QString &path)
{
    m_path = path;
    m_info = (ay_info_t*)calloc(sizeof(ay_info_t), 1);
    m_totalTime = 0;
}

AyflyHelper::~AyflyHelper()
{
    close();
}

void AyflyHelper::close()
{
    if(m_info) 
    {
        if(m_info->ay)
         {
            ay_closesong(&m_info->ay);
        }
        free(m_info);
    }
}

bool AyflyHelper::initialize()
{
    FILE *file = stdio_open(m_path.toLocal8Bit().constData());
    if(!file)
    {
        return false;
    }

    int size = stdio_length(file);
    if(size <= 0)
    {
        return false;
    }

    unsigned char *module = (unsigned char *)malloc(size);
    if(!module)
    {
        return false;
    }

    stdio_read(module, size, 1, file);
    stdio_close(file);

    if(!ay_initsongindirect(module, samplerate(), size))
    {
        free(module);
        return false;
    }
    free(module);

    m_info->ay = ay_initsong(m_path.toLocal8Bit().constData(), samplerate());
    if(!m_info->ay)
    {
        return false;
    }

    m_info->length = ay_getsonglength(m_info->ay) / 50 * 1000;
    m_info->rate = size * 8.0 / m_info->length;

    m_meta.insert("title", ay_getsongname(m_info->ay));
    m_meta.insert("artist", ay_getsongauthor(m_info->ay));

    return true;
}

int AyflyHelper::totalTime() const
{
    return m_info->length;
}

void AyflyHelper::seek(qint64 time)
{
    ay_seeksong(m_info->ay, time / 1000 * 50);
}

int AyflyHelper::bitrate() const
{
    return m_info->rate;
}

int AyflyHelper::samplerate() const
{
    return 44100;
}

int AyflyHelper::channels() const
{
    return 2;
}

int AyflyHelper::bitsPerSample() const
{
    return 16;
}

int AyflyHelper::read(unsigned char *buf, int size)
{
    return ay_rendersongbuffer(m_info->ay, buf, size);
}

QVariantMap AyflyHelper::readMetaTags()
{
    return m_meta;
}
