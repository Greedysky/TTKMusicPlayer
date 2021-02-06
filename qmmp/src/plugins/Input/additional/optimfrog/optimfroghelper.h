/* =================================================
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2021 Greedysky Studio

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
 ================================================= */

#ifndef OPTIMFROGHELPER_H
#define OPTIMFROGHELPER_H

#include <map>
#include <QFile>
#include <OptimFROG/OptimFROG.h>
#if defined Q_OS_WIN && defined __GNUC__
#include <qt_windows.h>
#endif

class OptimFROGHelper
{
public:
    explicit OptimFROGHelper(QIODevice *i);
    OptimFROGHelper(const OptimFROGHelper &) = delete;
    OptimFROGHelper &operator=(const OptimFROGHelper &) = delete;
    ~OptimFROGHelper();

    bool initialize();

    int read(void *buf, long size);
    void seek(int pos);

    int rate() const { return m_info.samplerate; }
    int channels() const { return m_info.channels; }
    int depth() const { return m_info.bitspersample; }
    int bitrate() const { return m_info.bitrate; }
    int version() const { return m_info.version; }
    int length() const;
    double compression() const { return 1000.0 * bitrate() / rate() / channels() / depth(); }

    bool hasTags() const { return !m_tags.empty(); }
    QString getTag(const char* tag) { return QString::fromStdString(m_tags[tag]); }

private:
#if defined Q_OS_WIN && defined __GNUC__
    FARPROC GetSymbolAddress(const char* name) const;
    HINSTANCE m_instance = nullptr;
#endif
    void *m_decoder;
    void *m_reader;
    OptimFROG_Info m_info;
    bool m_signed;

    std::map<std::string, std::string> m_tags;

    static QIODevice *VFS(void *instance) { return reinterpret_cast<QIODevice *>(instance); }

    static condition_t ofr_close(void *) { return true; }
    static sInt32_t ofr_read(void *instance, void *buf, uInt32_t n) { return VFS(instance)->read(reinterpret_cast<char *>(buf), n); }
    static condition_t ofr_eof(void* instance) { return VFS(instance)->atEnd(); }
    static condition_t ofr_seekable(void* instance) { return !VFS(instance)->isSequential(); }
    static sInt64_t ofr_length(void* instance) { return VFS(instance)->size(); }
    static sInt64_t ofr_get_pos(void* instance) { return VFS(instance)->pos(); }
    static condition_t ofr_seek(void* instance, sInt64_t offset) { return VFS(instance)->seek(offset); }

};

#endif
