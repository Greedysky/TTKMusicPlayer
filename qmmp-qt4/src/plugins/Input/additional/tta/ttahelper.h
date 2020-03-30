/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
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

#ifndef TTAHELPER_H
#define TTAHELPER_H

extern "C" {
#include "ttadec.h"
#include "stdio_meta_type.h"
}
#include <QVariantMap>

typedef struct {
    tta_info* tta;
    char* buffer;
    int remaining;

    int samples_to_skip;
    int currentsample;
    int startsample;
    int endsample;
    float readpos;
} tta_info_t;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class TTAHelper
{
public:
    TTAHelper(const QString &url);
    ~TTAHelper();

    void close();

    bool initialize();
    int totalTime() const;
    void seek(qint64 time);

    int bitrate() const;
    int samplerate() const;
    int channels() const;
    int bitsPerSample() const;

    int read(unsigned char *buf, int size);
    QVariantMap readTags(stdio_meta_type stdio_meta);

private:
    QString m_path;
    tta_info_t* m_info;
};

#endif // TTAHELPER_H
