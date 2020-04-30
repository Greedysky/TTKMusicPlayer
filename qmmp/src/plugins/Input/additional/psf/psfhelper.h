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

#ifndef PSFHELPER_H
#define PSFHELPER_H

#include <QVariantMap>

typedef struct {
    int type;
    void *decoder;
    char *filebuffer;
    size_t filesize;
    char buffer[735 * 4]; // psf2 decoder only works with 735 samples buffer
    int remaining;
    float duration;

    int currentsample;
    int samples_to_skip;
    float readpos;
} psf_info_t;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class PSFHelper
{
public:
    PSFHelper(const QString &path);
    virtual ~PSFHelper();

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
    inline QString album() const { return m_meta.value("album").toString(); }
    inline QString year() const { return m_meta.value("year").toString(); }

private:
    QString m_path;
    psf_info_t *m_info;
    QVariantMap m_meta;
};

#endif
