/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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
 ***************************************************************************/

#ifndef STSOUNDHELPER_H
#define STSOUNDHELPER_H

#include <QFile>
#include <qmmp/qmmp.h>
#include <libstsound/ym_music.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class StSoundHelper
{
public:
    explicit StSoundHelper(const QString &path);
    ~StSoundHelper();

    void deinit();
    bool initialize();

    inline void seek(qint64 time) { m_input->setMusicTime(time); }
    inline qint64 totalTime() const { return m_length; }

    inline int bitrate() const { return 8; }
    inline int sampleRate() const { return 44100; }
    inline int channels() const { return 2; }
    inline int depth() const { return 16; }

    qint64 read(unsigned char *data, qint64 maxSize);

    inline QString title() const { return m_title; }
    inline QString author() const { return m_author; }
    inline QString comment() const { return m_comment; }
    inline QString type() const { return m_songType; }
    inline QString player() const { return m_songPlayer; }
    inline int frames() const { return m_input->GetNbFrame(); }
    inline int streamInc() const { return m_input->GetStreamInc(); }

private:
    QString m_path;
    CYmMusic *m_input = nullptr;
    qint64 m_length = 0;
    QString m_title, m_author, m_comment;
    QString m_songType, m_songPlayer;

};

#endif
