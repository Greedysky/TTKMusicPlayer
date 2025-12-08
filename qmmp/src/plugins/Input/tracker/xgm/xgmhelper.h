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

#ifndef XGMHELPER_H
#define XGMHELPER_H

#include <QFile>
#include <qmmp/trackinfo.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class AbstractReader
{
public:
    AbstractReader(const QString &path);
    virtual ~AbstractReader() = default;

    virtual bool load() = 0;
    virtual qint64 read(unsigned char *data, qint64 maxSize) = 0;
    virtual void seek(qint64 time) = 0;
    virtual QList<TrackInfo*> createPlayList(TrackInfo::Parts parts) = 0;

    inline int bitrate() const { return 8; }
    inline int sampleRate() const { return 44100; }
    inline int channels() const { return 2; }
    inline int depth() const { return 16; }

    inline qint64 totalTime() const { return m_length; }

    QString cleanPath() const;

protected:
    QString m_path;
    qint64 m_length = 0;
    qint64 m_currentSample = 0;
    qint64 m_totalSamples = 0;

};


/*!
 * @author Greedysky <greedysky@163.com>
 */
class XGMHelper
{
public:
    explicit XGMHelper(const QString &path);
    ~XGMHelper();

    void deinit();
    bool initialize();

    inline void seek(qint64 time) { m_input->seek(time); }
    inline qint64 totalTime() const { return m_input->totalTime(); }

    inline int bitrate() const { return m_input->bitrate(); }
    inline int sampleRate() const { return m_input->sampleRate(); }
    inline int channels() const { return m_input->channels(); }
    inline int depth() const { return m_input->depth(); }

    inline qint64 read(unsigned char *data, qint64 maxSize) { return m_input->read(data, maxSize); }

    inline QList<TrackInfo*> createPlayList(TrackInfo::Parts parts) { return m_input->createPlayList(parts); }

private:
    AbstractReader *m_input = nullptr;

};

#endif
