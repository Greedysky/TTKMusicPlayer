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

#ifndef OPTIMFROGHELPER_H
#define OPTIMFROGHELPER_H

#include <QMap>
#include <QFile>
#include <QLibrary>
#include <OptimFROG/OptimFROG.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class OptimFROGHelper
{
public:
    explicit OptimFROGHelper(QIODevice *input);
    ~OptimFROGHelper();

    void deinit();
    bool initialize();

    void seek(qint64 time);
    qint64 totalTime() const;

    inline int bitrate() const { return m_info.bitrate; }
    inline int sampleRate() const { return m_info.samplerate; }
    inline int channels() const { return m_info.channels; }
    inline int depth() const { return m_info.bitspersample; }
    inline int version() const { return m_info.version; }

    qint64 read(unsigned char *data, qint64 maxSize);

    inline double compression() const { return 1000.0 * bitrate() / sampleRate() / channels() / depth(); }

    inline bool hasMetaData() const { return !m_metaData.isEmpty(); }
    inline QString metaData(const char *key) const { return m_metaData[key]; }

private:
    void *m_input = nullptr;
    void *m_decoder = nullptr;
    QLibrary *m_instance = nullptr;
    OptimFROG_Info m_info;
    bool m_signed = false;

    QMap<QString, QString> m_metaData;

    static QIODevice *VFS(void *instance) { return reinterpret_cast<QIODevice *>(instance); }

    static condition_t ofr_close(void *) { return true; }
    static sInt32_t ofr_read(void *instance, void *buf, uInt32_t n) { return VFS(instance)->read(reinterpret_cast<char *>(buf), n); }
    static condition_t ofr_eof(void *instance) { return VFS(instance)->atEnd(); }
    static condition_t ofr_seekable(void *instance) { return !VFS(instance)->isSequential(); }
    static sInt64_t ofr_length(void *instance) { return VFS(instance)->size(); }
    static sInt64_t ofr_get_pos(void *instance) { return VFS(instance)->pos(); }
    static condition_t ofr_seek(void *instance, sInt64_t offset) { return VFS(instance)->seek(offset); }

};

#endif
