/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

#ifndef QOAHELPER_H
#define QOAHELPER_H

#include <QFile>
#include <qmmp/qmmp.h>
extern "C" {
#include <libqoa/qoaplay.h>
}

/*!
 * @author Greedysky <greedysky@163.com>
 */
class QOAHelper
{
public:
    explicit QOAHelper(const QString &path);
    ~QOAHelper();

    void deinit();
    bool initialize();

    void seek(qint64 time);
    inline qint64 totalTime() const { return qoaplay_get_duration(m_input) * 1000; }

    inline int bitrate() const { return 8; }
    inline int sampleRate() const { return m_input->info.samplerate; }
    inline int channels() const { return m_input->info.channels; }
    inline int depth() const { return 32; }

    qint64 read(unsigned char *data, qint64 maxSize);

private:
    QString m_path;
    qoaplay_desc *m_input = nullptr;

};

#endif
