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

#ifndef ADPLUGHELPER_H
#define ADPLUGHELPER_H

#include <QFile>
#include <adplug/adplug.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class AdPlugHelper
{
public:
    struct Frame
    {
      Frame(size_t n, unsigned char *buf)
          : m_n(n),
            m_buf(buf)
      {

      }

      size_t m_n = 0;
      unsigned char *m_buf = nullptr;
    };

    explicit AdPlugHelper(const QString &path);
    ~AdPlugHelper();

    void deinit();
    inline bool initialize() { return m_player; }

    inline void seek(qint64 time) const { m_player->seek(time); }
    inline qint64 totalTime() const { return m_player->songlength(); }

    inline int bitrate() const { return 8; }
    inline int sampleRate() const { return m_sampleRate; }
    inline int channels() const { return 1; }
    inline int depth() const { return 16; }

    Frame read();

    inline QString title() const { return QString::fromStdString(m_player->gettitle()); }
    inline QString format() const { return QString::fromStdString(m_player->gettype()); }
    inline QString author() const { return QString::fromStdString(m_player->getauthor()); }
    inline QString description() const { return QString::fromStdString(m_player->getdesc()); }
    inline int patterns() const { return m_player->getpatterns(); }
    inline int instruments() const { return m_player->getinstruments(); }
    inline QString instrument(int i) const { return QString::fromStdString(m_player->getinstrument(i)); }

private:
    QString m_path;
    Copl *m_opl = nullptr;
    CPlayer *m_player = nullptr;
    short m_buf[16384] = { 0 };
    size_t m_remaining = 0;
    int m_sampleRate = 44100;

};

#endif
