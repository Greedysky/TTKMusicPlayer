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

#ifndef ASAPHELPER_H
#define ASAPHELPER_H

extern "C" {
#include "asap.h"
#include "stdio_file.h"
}
#include <QMap>
#include <QString>

typedef struct {
    ASAP *asap;
    int sample_rate;
    int length;
    int bitrate;
    int bits_per_sample;
    int channels;
} asap_info_t;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class AsapHelper
{
public:
    explicit AsapHelper(const QString &path);
    ~AsapHelper();

    void close();

    bool initialize();
    int totalTime() const;
    void seek(qint64 time);

    int bitrate() const;
    int sampleRate() const;
    int channels() const;
    int bitsPerSample() const;

    int read(unsigned char *buf, int size);
    QMap<QString, QString> readMetaTags() const;

    inline QString title() const { return m_meta.value("title"); }
    inline QString artist() const { return m_meta.value("artist"); }
    inline QString year() const { return m_meta.value("year"); }

private:
    QString m_path;
    asap_info_t *m_info;
    QMap<QString, QString> m_meta;

};

#endif
