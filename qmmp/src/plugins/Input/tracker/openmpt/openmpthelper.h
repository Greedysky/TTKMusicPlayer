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

#ifndef OPENMPTHELPER_H
#define OPENMPTHELPER_H

#include <QMap>
#include <QIODevice>
#include <QStringList>
#include <libopenmpt/libopenmpt.h>
#include <libopenmpt/libopenmpt_stream_callbacks_file.h>

#define INTERP_NONE     1
#define INTERP_LINEAR   2
#define INTERP_CUBIC    4
#define INTERP_WINDOWED 8

/*!
 * @author Greedysky <greedysky@163.com>
 */
class OpenMPTHelper
{
public:
    explicit OpenMPTHelper(QIODevice *input);
    ~OpenMPTHelper();

    void deinit();
    bool initialize();

    static QMap<QString, int> interpolators();
    bool isValidInterpolator(int value) const;
    void setInterpolator(int value);
    bool isValidStereoSeparation(int separation) const;
    void setStereoSeparation(int separation);

    inline void seek(qint64 time) { openmpt_module_set_position_seconds(m_mod, time / 1000.0); }
    inline qint64 totalTime() const { return m_length; }

    inline int bitrate() const { return 8; }
    inline int sampleRate() const { return 44100; }
    inline int channels() const { return 2; }
    inline int depth() const { return 16; }

    qint64 read(unsigned char *data, qint64 maxSize);

    inline QString title() const { return m_title; }
    inline QString comment() const { return m_comment; }

    inline int patternCount() const { return m_patternCount; }
    inline int channelCount() const { return m_channelCount; }
    inline int sampleCount() const { return m_sampleCount; }
    inline int instrumentCount() const { return m_instrumentCount; }

    inline QString instruments() const { return m_instruments; }
    inline QString samples() const { return m_samples; }

private:
    QString toString(const char *input);

    static std::size_t stream_read(void *, void *, std::size_t);
    static int stream_seek(void *, std::int64_t, int);
    static std::int64_t stream_tell(void *);
    static QIODevice *VFS(void *instance) { return reinterpret_cast<QIODevice *>(instance); }

    openmpt_stream_callbacks callbacks = { stream_read, stream_seek, stream_tell };

    QIODevice *m_input = nullptr;
    openmpt_module *m_mod = nullptr;
    qint64 m_length = 0;
    QString m_title;
    QString m_comment;
    int m_patternCount = 0, m_channelCount = 0;
    int m_instrumentCount = 0, m_sampleCount = 0;
    QString m_instruments;
    QString m_samples;

};

#endif
