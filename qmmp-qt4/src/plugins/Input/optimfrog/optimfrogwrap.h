/***************************************************************************
 *   Copyright (C) 2006-2019 by Ilya Kotov                                 *
 *   forkotov02@ya.ru                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#ifndef OPTIMFROGWRAP_H
#define OPTIMFROGWRAP_H

#include <map>
#include <exception>
#include <QFile>
#include <OptimFROG/OptimFROG.h>

class OptimFROGWrap
{
public:
    class InvalidFile : public std::exception
    {
      public:
        InvalidFile() : std::exception() { }
    };

    explicit OptimFROGWrap(QIODevice *device);
    OptimFROGWrap(const OptimFROGWrap &) = delete;
    OptimFROGWrap &operator=(const OptimFROGWrap &) = delete;
    ~OptimFROGWrap();

    static bool canPlay(const std::string &name);

    int read(void *buf, long size);
    void seek(int pos);

    int rate() const { return m_info.samplerate; }
    int channels() const { return m_info.channels; }
    int depth() const { return m_info.bitspersample; }
    int length() const { return m_info.length_ms; }
    int bitrate() const { return m_info.bitrate; }
    int version() const { return m_info.version; }
    double compression() const { return 1000.0 * bitrate() / rate() / channels() / depth(); }

    bool hasTags() const { return !m_tags.empty(); }
    std::string getTag(const std::string& tag) const { return m_tags.at(tag); }

private:
    void *decoder;
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
