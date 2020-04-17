/******************************************************************************
 * Copyright (c) 2015-2016, Georges Thill
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#include <QFileInfo>
#include "decoder_ym.h"

DecoderYm::DecoderYm(const QString &path) : Decoder()
{
    m_path = path;
    m_bitrate = 0;
    m_totalTime = 0;
    m_freq = 0;
    m_music = nullptr;
}

DecoderYm::~DecoderYm()
{
    deinit();
}

bool DecoderYm::initialize()
{
    m_totalTime = 0;
    m_freq = 0;
    m_bitrate = 0;

    m_music = new CYmMusic;
    if(m_music == nullptr)
    {
        qWarning("DecoderYm: failed to create CYmMusic");
        return false;
    }

    ymMusicInfo_t info;
    if(m_music->load(m_path.toLocal8Bit().constData()))
    {
        m_music->getMusicInfo(&info);
        m_music->setLoopMode(YMFALSE);

        m_totalTime = info.musicTimeInMs;
        m_freq = 44100;
        m_bitrate = (QFileInfo(m_path).size () * 8.0) / m_totalTime + 0.5;
    }
    else
    {
        if(m_music)
        {
            delete m_music;
        }
        m_music = nullptr;
        qWarning("DecoderYm: failed to open: %s", qPrintable(m_path));
        return false;
    }

    configure(m_freq, 2, Qmmp::PCM_S16LE);
    
    qDebug("DecoderYm: detected format: \"%s\"", info.pSongType);
    qDebug("DecoderYm: initialize success");
    return true;
}

void DecoderYm::deinit()
{
    m_totalTime = 0;
    m_bitrate = 0;
    m_freq = 0;
    if(m_music)
    {
        delete m_music;
    }
    m_music = nullptr;
}

qint64 DecoderYm::totalTime() const
{
    return m_totalTime;
}

int DecoderYm::bitrate() const
{
    return m_bitrate ? m_bitrate : 0;
}

qint64 DecoderYm::read(unsigned char *data, qint64 size)
{
    qint64 stereoSize, i;
    ymsample *psample=(ymsample *)data;

    stereoSize = size / (2*sizeof(ymsample));
    
    if(m_music->update(psample, stereoSize))
    {
        // recopy mono YM sound to 2 channels
        for(i=stereoSize-1; i>=0; i--)
        {
            psample[(i*2)    ] = psample[i];
            psample[(i*2) + 1] = psample[i];
        }

        return size;
    }

    return 0;
}

void DecoderYm::seek(qint64 pos)
{
    m_music->setMusicTime((ymu32)pos);
}
