/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

#ifndef SUNVOXHELPER_H
#define SUNVOXHELPER_H

#include <QFile>
#include <QLibrary>
#include <QStringList>
#include <qmmp/qmmp.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class SunVoxHelper
{
public:
    explicit SunVoxHelper(const QString &path);
    ~SunVoxHelper();

    void deinit();
    bool initialize(bool copy = false);

    void seek(qint64 time);
    qint64 totalTime() const;

    inline int bitrate() const { return 8; }
    inline int sampleRate() const { return 44100; }
    inline int channels() const { return 2; }
    inline int depth() const { return 16; }

    qint64 read(unsigned char *data, qint64 maxSize);

    inline QString title() const { return m_title; }

    inline int beatsPerMinute() const { return m_beatsPerMinute; }
    inline int ticksPerLine() const { return m_ticksPerLine; }
    inline int moduleCount() const { return m_moduleCount; }
    inline int patternCount() const { return m_patternCount; }

    inline QString modules() const { return m_modules; }
    inline QString patterns() const { return m_patterns; }

private:
    QString m_path;
    bool m_copyMode = false;
    QLibrary *m_instance = nullptr;
    QString m_title;
    int m_beatsPerMinute = 0, m_ticksPerLine = 0;
    int m_moduleCount = 0, m_patternCount = 0;
    QString m_modules;
    QString m_patterns;

};

#endif
