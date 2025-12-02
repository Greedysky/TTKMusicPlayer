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

#ifndef SOUNDMONHELPER_H
#define SOUNDMONHELPER_H

#include <QFile>
#include <qmmp/qmmp.h>
#include <libsoundmon/player.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class SoundMonHelper
{
public:
    explicit SoundMonHelper(const QString &path);
    ~SoundMonHelper();

    void deinit();
    bool initialize();

    inline void seek(qint64 time) { return m_input->Seek(time); }
    inline qint64 totalTime() const { return m_input->GetLength(); }

    inline int bitrate() const { return 8; }
    inline int sampleRate() const { return SAMPLERATE; }
    inline int channels() const { return 2; }
    inline int depth() const { return 8; }

    #undef SAMPLERATE
    qint64 read(unsigned char *data, qint64 maxSize);

private:
    QString m_path;
    Player *m_input = nullptr;

};

#endif
