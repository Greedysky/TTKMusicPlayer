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

#ifndef PSFHELPER_H
#define PSFHELPER_H

#include <QMap>
#include <QFile>
#include <qmmp/qmmp.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class PSFHelper
{
public:
    explicit PSFHelper(const QString &path);
     ~PSFHelper();

    void deinit();
    bool initialize();

    void seek(qint64 time);
    inline qint64 totalTime() const { return m_length * 1000; }

    inline int bitrate() const { return 8; }
    inline int sampleRate() const { return 44100; }
    inline int channels() const { return 2; }
    inline int depth() const { return 16; }

    qint64 read(unsigned char *data, qint64 maxSize);

    inline bool hasMetaData() const { return !m_metaData.isEmpty(); }
    inline QString metaData(const char *key) const { return m_metaData[key]; }

private:
    QString m_path;
    int m_type = 0;
    void *m_input = nullptr;
    char *m_data = nullptr;
    char m_buffer[735 * 4] = {0}; // psf2 decoder only works with 735 samples buffer
    int m_remaining = 0;
    qint64 m_length = 0;
    qint64 m_currentSample = 0;
    qint64 m_samplesToSkip = 0;
    QMap<QString, QString> m_metaData;

};

#endif
