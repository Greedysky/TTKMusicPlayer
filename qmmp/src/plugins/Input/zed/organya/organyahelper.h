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

#ifndef ORGANYAHELPER_H
#define ORGANYAHELPER_H

#include <QMap>
#include <QFile>
#include <qmmp/qmmp.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class AbstractReader
{
public:
    AbstractReader() { }
    virtual ~AbstractReader() { }

    virtual bool load(const QString &path) = 0;
    virtual void seek(qint64 time) = 0;
    virtual qint64 totalTime() const = 0;
    virtual qint64 read(unsigned char *data, qint64 maxSize) = 0;

    inline int sampleRate() const { return 44100; }
    inline int channels() const { return 2; }

};


/*!
 * @author Greedysky <greedysky@163.com>
 */
class OrganyaHelper
{
public:
    explicit OrganyaHelper(const QString &path);
    ~OrganyaHelper();

    void deinit();
    bool initialize();

    inline void seek(qint64 time) { m_input->seek(time); }
    inline qint64 totalTime() const { return m_input->totalTime(); }

    inline int bitrate() const { return 8; }
    inline int sampleRate() const { return m_input->sampleRate(); }
    inline int channels() const { return m_input->channels(); }
    inline int depth() const { return 16; }

    inline qint64 read(unsigned char *data, qint64 maxSize) { return m_input->read(data, maxSize); }

private:
    QString m_path;
    AbstractReader *m_input = nullptr;

};

#endif
