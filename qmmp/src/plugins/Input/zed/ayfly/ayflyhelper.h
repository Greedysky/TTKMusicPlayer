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

#ifndef AYFLYHELPER_H
#define AYFLYHELPER_H

#include <QMap>
#include <QFile>
#include <qmmp/qmmp.h>
#include <libayfly/ayfly.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class AyflyHelper
{
public:
    explicit AyflyHelper(const QString &path);
    ~AyflyHelper();

    void deinit();
    bool initialize();

    inline void seek(qint64 time) { ay_seeksong(m_input, time / 1000 * 50); }
    inline qint64 totalTime() const { return m_length; }

    inline int bitrate() const { return m_bitrate; }
    inline int sampleRate() const { return 44100; }
    inline int channels() const { return 2; }
    inline int depth() const { return 16; }

    inline qint64 read(unsigned char *data, qint64 maxSize) { return ay_rendersongbuffer(m_input, data, maxSize); }
    inline const QMap<Qmmp::MetaData, QString> &readMetaData() const { return m_metaData; }

private:
    QString m_path;
    void *m_input = nullptr;
    int m_length = 0;
    int m_bitrate = 0;
    QMap<Qmmp::MetaData, QString> m_metaData;

};

#endif