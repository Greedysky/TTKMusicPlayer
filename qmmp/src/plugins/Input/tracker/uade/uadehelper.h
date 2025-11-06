/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

#ifndef UADEHELPER_H
#define UADEHELPER_H

#include <QFileInfo>
#include <QStringList>
#include <qmmp/trackinfo.h>
#include <uade/uade.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class UADEHelper
{
public:
    explicit UADEHelper(const QString &path);
    ~UADEHelper();

    void deinit();
    bool initialize();

    void seek(qint64 time);
    qint64 totalTime();

    inline int bitrate() const { return 8; }
    inline int sampleRate() const { return 44100; }
    inline int channels() const { return 2; }
    inline int depth() const { return 16; }

    qint64 read(unsigned char *data, qint64 maxSize);

    QList<TrackInfo*> createPlayList(TrackInfo::Parts parts);
    QString cleanPath() const;

    static QStringList filters();

    inline bool hasMetaData() const { return !m_metaData.isEmpty(); }
    inline QString metaData(const char *key) const { return m_metaData[key]; }

private:
    QString m_path;
    int m_track = 0;
    QMap<QString, QString> m_metaData;
    struct uade_state* m_state = nullptr;

};

#endif
