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

#ifndef VTXHELPER_H
#define VTXHELPER_H

#include <QObject>

extern "C" {
#include "ayemu.h"
}

#define AY_FRAME_SIZE 14

typedef struct {
    ayemu_vtx_t *decoder;
    ayemu_ay_t ay;
    unsigned char regs[AY_FRAME_SIZE];
    int vtx_pos;
    int left;
    int rate;
    int currentsample;
    float readpos;
} vtx_info_t;


class VTXHelper
{
public:
    VTXHelper(const QString &path);
    virtual ~VTXHelper();

    void close();

    bool initialize();
    int totalTime() const;
    void seek(qint64 time);

    int bitrate() const;
    int samplerate() const;
    int channels() const;
    int bitsPerSample() const;

    int read(unsigned char *buf, int size);

    inline QString title() const { return m_title; }
    inline QString artist() const { return m_artist; }
    inline QString album() const { return m_album; }
    inline QString tracker() const { return m_tracker; }
    inline QString comment() const { return m_comment; }

private:
    QString m_path;
    vtx_info_t *m_info;
    qint64 m_totalTime;
    QString m_title, m_album, m_artist;
    QString m_tracker, m_comment;
};

#endif
