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
    virtual ~PSFHelper();

    void deinit();
    bool initialize();

    void seek(qint64 time);
    inline qint64 totalTime() const { return m_length * 1000; }

    inline int bitrate() const { return m_bitrate; }
    inline int sampleRate() const { return 44100; }
    inline int channels() const { return 2; }
    inline int depth() const { return 16; }

    qint64 read(unsigned char *data, qint64 maxSize);
    QMap<Qmmp::MetaData, QString> readMetaData() const;

private:
    QString m_path;
    int m_type = 0;
    void *m_input = nullptr;
    int m_bitrate = 0;
    char m_buffer[735 * 4] = {0}; // psf2 decoder only works with 735 samples buffer
    int m_remaining = 0;
    int m_length = 0;
    int m_current_sample = 0;
    int m_samples_to_skip = 0;

};

#endif