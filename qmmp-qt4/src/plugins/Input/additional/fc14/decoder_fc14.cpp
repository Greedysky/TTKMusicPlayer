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

#include "decoder_fc14.h"

DecoderFC14::DecoderFC14(const QString &path)
    : Decoder()
{
//    m_path = path;
//    m_bitrate = 0;
//    m_totalTime = 0;
//    m_freq = 0;
//    m_music = nullptr;
}

DecoderFC14::~DecoderFC14()
{
//    deinit();
}

bool DecoderFC14::initialize()
{
//    m_totalTime = 0;
//    m_freq = 0;
//    m_bitrate = 0;

//    m_music = new CYmMusic;
//    if(m_music == nullptr)
//    {
//        qWarning("DecoderYm: failed to create CYmMusic");
//        return false;
//    }

//    ymMusicInfo_t info;
//    if(m_music->load(m_path.toLocal8Bit().constData()))
//    {
//        m_music->getMusicInfo(&info);
//        m_music->setLoopMode(YMFALSE);

//        m_totalTime = info.musicTimeInMs;
//        m_freq = 44100;
//        m_bitrate = ((QFileInfo(m_path).size () * 8.0) / m_totalTime) + 0.5;
//    }
//    else
//    {
//        if(m_music)
//        {
//            delete m_music;
//        }
//        m_music = nullptr;
//        qWarning("DecoderYm: failed to open: %s", qPrintable(m_path));
//        return false;
//    }

//    configure(m_freq, 2, Qmmp::PCM_S16LE);
    
//    qDebug("DecoderYm: detected format: \"%s\"", info.pSongType);
//    qDebug("DecoderYm: initialize success");
    return true;
}

qint64 DecoderFC14::totalTime() const
{
    return 0;
}

int DecoderFC14::bitrate() const
{
    return 1;
}

qint64 DecoderFC14::read(unsigned char *data, qint64 size)
{
    return 0;
}

void DecoderFC14::seek(qint64 pos)
{

}
