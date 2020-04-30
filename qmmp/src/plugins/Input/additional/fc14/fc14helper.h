/* =================================================
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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
 ================================================= */

#ifndef FC14HELPER_H
#define FC14HELPER_H

extern "C" {
#include "fc14audiodecoder.h"
#include "stdio_file.h"
}
#include <QVariantMap>

typedef struct {
    void *fc;
    int sample_rate;
    int length;
    int bitrate;
    int bits_per_sample;
    int channels;
} fc14_info_t;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class FC14Helper
{
public:
    FC14Helper(const QString &path);
    virtual ~FC14Helper();

    void close();

    bool initialize();
    int totalTime() const;
    void seek(qint64 time);

    int bitrate() const;
    int samplerate() const;
    int channels() const;
    int bitsPerSample() const;

    int read(unsigned char *buf, int size);
    QVariantMap readMetaTags();

private:
    QString m_path;
    fc14_info_t *m_info;
};

#endif
