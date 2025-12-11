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

#ifndef AYFLYHELPER_H
#define AYFLYHELPER_H

#include <QFile>
#include <qmmp/qmmp.h>
#undef _UNICODE
#include <libayfly/ayfly.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class AyflyHelper
{
public:
    explicit AyflyHelper(const QString &path);
    ~AyflyHelper();

    void deinit();
    bool initialize();

    inline void seek(qint64 time) { ay_seeksong(m_input, time * 50 / 1000); }
    inline qint64 totalTime() const { return ay_getsonglength(m_input) * 1000 / 50; }

    inline int bitrate() const { return 8; }
    inline int sampleRate() const { return 44100; }
    inline int channels() const { return 2; }
    inline int depth() const { return 16; }

    inline qint64 read(unsigned char *data, qint64 maxSize) { return ay_rendersongbuffer(m_input, data, maxSize); }

    inline QString title() const { return ay_getsongname(m_input); }
    inline QString author() const { return ay_getsongauthor(m_input); }

private:
    QString m_path;
    void *m_input = nullptr;

};

#endif
