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

#ifndef HIVELYHELPER_H
#define HIVELYHELPER_H

#include <QFile>
#include <qmmp/qmmp.h>
extern "C" {
#include <libhively/hvl_replay.h>
}

/*!
 * @author Greedysky <greedysky@163.com>
 */
class HivelyHelper
{
public:
    explicit HivelyHelper(const QString &path);
    ~HivelyHelper();

    void deinit();
    bool initialize();

    inline void seek(qint64 time) { hvl_Seek(m_input, time); }
    inline qint64 totalTime() const { return hvl_GetPlayTime(m_input); }

    inline int bitrate() const { return 8; }
    inline int sampleRate() const { return 44100; }
    inline int channels() const { return 2; }
    inline int depth() const { return 16; }

    qint64 read(unsigned char *data, qint64 maxSize);

    inline QString title() const { return m_input->ht_Name; }
    inline int subSongCount() const { return m_input->ht_SubsongNr; }
    inline int instrumentCount() const { return m_input->ht_InstrumentNr; }

    QString format() const;
    QString subSongs() const;
    QString instruments() const;

private:
    QString m_path;
    bool m_ahxHeader = true;
    struct hvl_tune *m_input = nullptr;

};

#endif
