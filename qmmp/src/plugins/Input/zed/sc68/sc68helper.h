/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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

#ifndef SC68HELPER_H
#define SC68HELPER_H

#include <QFile>
#include <qmmp/trackinfo.h>
extern "C" {
#include <libsc68/sc68/sc68.h>
}

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

    void seek(qint64 time);
    inline qint64 totalTime() const { return m_length; }

    inline int bitrate() const { return 8; }
    inline int sampleRate() const { return 44100; }
    inline int channels() const { return 2; }
    inline int depth() const { return 16; }

    qint64 read(unsigned char *data, qint64 maxSize);

    QList<TrackInfo*> createPlayList(TrackInfo::Parts parts);
    QString cleanPath() const;

private:
    QString m_path;
    sc68_t *m_input = nullptr;
    int m_track = 0;
    int m_loop = 0;
    int m_length = 0;
    int m_current_sample = 0;
    int m_total_samples = 0;

};

#endif
