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

#ifndef BPHELPER_H
#define BPHELPER_H

#include <QMap>
#include <QFile>
#include <qmmp/qmmp.h>

class Player;
typedef struct {
    Player *input;
    int bitrate;
} decode_info;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class BpHelper
{
public:
    explicit BpHelper(const QString &path);
    ~BpHelper();

    void deinit();

    bool initialize();
    int totalTime() const;
    void seek(qint64 time);

    int bitrate() const;
    int sampleRate() const;
    int channels() const;
    int bitsPerSample() const;

    int read(unsigned char *buf, int size);

private:
    QString m_path;
    decode_info *m_info;
};

#endif
