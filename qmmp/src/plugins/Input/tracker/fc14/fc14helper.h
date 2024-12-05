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

#ifndef FC14HELPER_H
#define FC14HELPER_H

#include <QFile>
#include <qmmp/qmmp.h>
#include <libfc14/fc14audiodecoder.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class FC14Helper
{
public:
    explicit FC14Helper(const QString &path);
    ~FC14Helper();

    void deinit();
    bool initialize();

    inline void seek(qint64 time) { fc14dec_seek(m_input, time); }
    inline qint64 totalTime() const { return fc14dec_duration(m_input); }

    inline int bitrate() const { return 8; }
    inline int sampleRate() const { return 44100; }
    inline int channels() const { return 2; }
    inline int depth() const { return 16; }

    qint64 read(unsigned char *data, qint64 maxSize);

    inline QString comment() const { return fc14dec_format_name(m_input); }

private:
    QString m_path;
    void *m_input = nullptr;

};

#endif
