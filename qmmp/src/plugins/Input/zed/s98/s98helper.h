/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

#ifndef S98HELPER_H
#define S98HELPER_H

#include <QFile>
#include <qmmp/trackinfo.h>
#include <libs98/s98.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class S98Helper
{
public:
    explicit S98Helper(const QString &path);
    ~S98Helper();

    void deinit();
    bool initialize();

    inline void seek(qint64 time) { m_input->SetPosition(time); }
    inline qint64 totalTime() const { return m_info.dwLength; }

    inline int bitrate() const { return 8; }
    inline int sampleRate() const { return 44100; }
    inline int channels() const { return 2; }
    inline int depth() const { return 16; }

    qint64 read(unsigned char *data, qint64 maxSize);

    inline bool hasTags() const { return !m_tags.isEmpty(); }
    inline QString tag(const char *key) const { return m_tags[key]; }

private:
    QString m_path;
    SOUNDINFO m_info;
    s98File *m_input = nullptr;
    QMap<QString, QString> m_tags;

};

#endif
