/* =================================================
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2021 Greedysky Studio

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

#ifndef SC68HELPER_H
#define SC68HELPER_H

extern "C" {
#include "stdio_file.h"
#include "sc68.h"
}
#include <qmmp/trackinfo.h>

typedef struct {
    sc68_t *sc68;
    int track;
    int loop;
    int bitrate;
    int length;
    int currentsample;
    int totalsamples;
} sc68_info_t;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class SC68Helper
{
public:
    explicit SC68Helper(const QString &path);
    ~SC68Helper();

    void close();

    bool initialize();
    int totalTime() const;
    void seek(qint64 time);

    int bitrate() const;
    int sampleRate() const;
    int channels() const;
    int bitsPerSample() const;

    int read(unsigned char *buf, int size);

    QList<TrackInfo*> createPlayList(TrackInfo::Parts parts);
    QString cleanPath() const;

private:
    QString m_path;
    sc68_info_t *m_info;

};

#endif
