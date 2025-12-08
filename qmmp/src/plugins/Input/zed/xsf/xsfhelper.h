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

#ifndef XSFHELPER_H
#define XSFHELPER_H

#include <QFile>
#include <qmmp/qmmp.h>
#include <libxsf/psflib/psfcore.h>

#define INPUT_BUFFER_SIZE   1024

/*!
 * @author Greedysky <greedysky@163.com>
 */
class XSFHelper
{
public:
    explicit XSFHelper(const QString &path, bool meta = false);
    ~XSFHelper();

    void deinit();
    bool initialize();

    inline void seek(qint64 time) { m_input->seek(time); }
    inline qint64 totalTime() const { return m_input->length(); }

    inline int bitrate() const { return 8; }
    inline int sampleRate() const { return m_input->rate(); }
    inline int channels() const { return 2; }
    inline int depth() const { return 16; }

    inline qint64 read(unsigned char *data, qint64) { return m_input->read((short*)data, INPUT_BUFFER_SIZE) * channels() * depth() / 8; }

    inline bool hasTags() const { return !m_input->get_meta_map().empty(); }
    inline QString tag(const char *key) const { return QString::fromStdString(m_input->get_meta_map()[key]); }

private:
    QString m_path;
    bool m_meta = false;
    AbstractReader *m_input = nullptr;

};

#endif
