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
#include <libsc68/sc68/sc68.h>
}
#include <qmmp/trackinfo.h>

typedef struct {
    sc68_t *input;
    int track;
    int loop;
    int bitrate;
    int length;
    int current_sample;
    int total_samples;
} decode_info;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class SC68Helper
{
public:
    explicit SC68Helper(const QString &path);
    ~SC68Helper();

    void deinit();

    bool initialize();
    qint64 totalTime() const;
    void seek(qint64 time);

    int bitrate() const;
    int sampleRate() const;
    int channels() const;
    int bitsPerSample() const;

    qint64 read(unsigned char *data, qint64 maxSize);

    QList<TrackInfo*> createPlayList(TrackInfo::Parts parts);
    QString cleanPath() const;

private:
    QString m_path;
    decode_info *m_info = nullptr;

};

#endif
