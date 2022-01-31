/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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

#include <QMap>
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

    inline int bitrate() const { return m_bitrate; }
    inline int sampleRate() const { return 44100; }
    inline int channels() const { return 2; }
    inline int depth() const { return 16; }

    qint64 read(unsigned char *data, qint64 maxSize);
    inline const QMap<Qmmp::MetaData, QString> &readMetaData() const { return m_metaData; }

private:
    QString m_path;
    struct hvl_tune *m_input = nullptr;
    int m_bitrate = 0;
    QMap<Qmmp::MetaData, QString> m_metaData;

};

#endif
