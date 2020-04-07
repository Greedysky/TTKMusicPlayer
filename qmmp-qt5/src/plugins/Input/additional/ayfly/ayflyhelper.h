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

#ifndef AYFLYHELPER_H
#define AYFLYHELPER_H

extern "C" {
#include "ayfly/ayfly.h"
#include "stdio_file.h"
}
#include <QVariantMap>

typedef struct {
    void * ay;
    int rate;
    int length;
} ay_info_t;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class AyflyHelper
{
public:
    AyflyHelper(const QString &path);
    virtual ~AyflyHelper();

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

    inline QString title() const { return m_meta.value("title").toString(); }
    inline QString artist() const { return m_meta.value("artist").toString(); }

private:
    QString m_path;
    ay_info_t *m_info;
    qint64 m_totalTime;
    QVariantMap m_meta;
};

#endif
