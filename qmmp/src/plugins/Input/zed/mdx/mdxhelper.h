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

#ifndef MDXHELPER_H
#define MDXHELPER_H

#include <QMap>
#include <QFile>
#include <qmmp/qmmp.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class FileReader
{
public:
    FileReader() { }
    virtual ~FileReader() { }

    virtual bool load(const QString &path) = 0;
    virtual qint64 totalTime() const = 0;
    virtual qint64 read(unsigned char *data, qint64 maxSize) = 0;

    inline int sampleRate() const { return 44100; }

    inline QString title() const { return m_title; }
    inline QString author() const { return m_author; }

protected:
    int m_offset = 0;
    int m_length = 0;
    QString m_title;
    QString m_author;

};


/*!
 * @author Greedysky <greedysky@163.com>
 */
class MDXHelper
{
public:
    explicit MDXHelper(const QString &path);
    ~MDXHelper();

    void deinit();
    bool initialize();

    inline qint64 totalTime() const { return m_input->totalTime(); }

    inline int bitrate() const { return 8; }
    inline int sampleRate() const { return m_input->sampleRate(); }
    inline int channels() const { return 2; }
    inline int depth() const { return 16; }

    inline qint64 read(unsigned char *data, qint64 maxSize) { return m_input->read(data, maxSize); }

    inline QString title() const { return m_input->title(); }
    inline QString author() const { return m_input->author(); }

private:
    QString m_path;
    FileReader *m_input = nullptr;

};

#endif
