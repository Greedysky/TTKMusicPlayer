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
extern "C" {
#include <libmdx/mdxmini/mdxmini.h>
#include <libmdx/pmdmini/pmdmini.h>
}

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

    inline void seek(qint64 time) { Q_UNUSED(time); }
    inline qint64 totalTime() const { return m_length; }

    inline int bitrate() const { return m_bitrate; }
    inline int sampleRate() const { return 44100; }
    inline int channels() const { return 2; }
    inline int depth() const { return 16; }

    qint64 read(unsigned char *data, qint64 maxSize);
    inline const QMap<Qmmp::MetaData, QString> &readMetaData() const { return m_metaData; }

private:
    QString m_path;
    t_mdxmini m_input;
    bool m_mdx_mode = false;
    int m_pos = 0;
    int m_bitrate = 0;
    int m_length = 0;
    QMap<Qmmp::MetaData, QString> m_metaData;

};

#endif
