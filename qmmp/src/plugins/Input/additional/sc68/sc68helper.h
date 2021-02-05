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
#include <QVariantMap>

typedef struct {
    sc68_t *sc68;
    int trk;
    int loop;
    int bitrate;
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
    QVariantMap readMetaTags();

    inline QString title() const { return m_meta.value("title").toString(); }
    inline QString artist() const { return m_meta.value("artist").toString(); }
    inline QString album() const { return m_meta.value("album").toString(); }
    inline QString year() const { return m_meta.value("year").toString(); }
    inline QString genre() const { return m_meta.value("genre").toString(); }
    inline QString track() const { return m_meta.value("track").toString(); }

private:
    QString m_path;
    sc68_info_t *m_info;
    qint64 m_totalTime = 0;
    QVariantMap m_meta;

};

#endif
