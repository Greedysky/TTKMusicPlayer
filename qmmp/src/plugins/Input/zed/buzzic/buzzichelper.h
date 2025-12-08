/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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

#ifndef BUZZICHELPER_H
#define BUZZICHELPER_H

#include <QFile>
#include <qmmp/qmmp.h>
#include <libbuzzic/Buzzic2.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class BuzzicHelper
{
public:
    explicit BuzzicHelper(const QString &path);
    ~BuzzicHelper();

    void deinit();
    bool initialize();

    void seek(qint64 time);
    inline qint64 totalTime() const { return Buzzic2DurationMs(m_input); }

    inline int bitrate() const { return 8; }
    inline int sampleRate() const { return 44100; }
    inline int channels() const { return 2; }
    inline int depth() const { return 32; }

    qint64 read(unsigned char *data, qint64 maxSize);

    QString instruments() const;
    uint32_t instrumentCount() const;

private:
    QString m_path;
    Buzzic2 *m_input = nullptr;
    qint64 m_currentSample = 0;
    qint64 m_totalSamples = 0;

};

#endif
