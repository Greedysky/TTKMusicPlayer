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

#ifndef XSFHELPER_H
#define XSFHELPER_H

#include <QMap>
#include <QFile>
#include <qmmp/qmmp.h>
#include "xsfreader.h"

#define SAMPLE_BUF_SIZE     1024

/*!
 * @author Greedysky <greedysky@163.com>
 */
class XSFHelper
{
public:
    explicit XSFHelper(const QString &path);
    ~XSFHelper();

    void metaOnly(bool meta);

    void deinit();
    bool initialize();

    inline void seek(qint64 time) { m_input->seek(time); }
    inline qint64 totalTime() const { return m_input->length(); }

    inline int bitrate() const { return m_bitrate; }
    inline int sampleRate() const { return 44100; }
    inline int channels() const { return 2; }
    inline int depth() const { return 16; }

    inline qint64 read(unsigned char *data, qint64 maxSize) { return m_input->read((short*)data, SAMPLE_BUF_SIZE) * 4; }
    QMap<Qmmp::MetaData, QString> readMetaData() const;

private:
    QString m_path;
    FileReader *m_input = nullptr;
    int m_bitrate = 0;
    bool m_meta = false;

};

#endif
