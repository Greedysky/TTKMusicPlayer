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

#ifndef ASAPHELPER_H
#define ASAPHELPER_H

#include <QFile>
#include <qmmp/qmmp.h>
#include <libasap/asap.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class AsapHelper
{
public:
    explicit AsapHelper(const QString &path);
    ~AsapHelper();

    void deinit();
    bool initialize();

    inline void seek(qint64 time) { ASAP_Seek(m_input, time); }
    inline qint64 totalTime() const { return m_length; }

    inline int bitrate() const { return 8; }
    inline int sampleRate() const { return ASAP_SAMPLE_RATE; }
    inline int channels() const { return m_channels; }
    inline int depth() const { return 16; }

    qint64 read(unsigned char *data, qint64 maxSize);

    inline QString title() const { return m_title; }
    inline QString author() const { return m_author; }
    inline QString year() const { return m_year; }

private:
    QString m_path;
    ASAP *m_input = nullptr;
    qint64 m_length = 0;
    int m_channels = 0;
    QString m_title;
    QString m_author;
    QString m_year;

};

#endif
