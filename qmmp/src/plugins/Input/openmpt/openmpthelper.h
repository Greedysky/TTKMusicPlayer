/***************************************************************************
 *   Copyright (C) 2013-2021 by Ilya Kotov                                 *
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

#ifndef OPENMPTHELPER_H
#define OPENMPTHELPER_H

#include <QMap>
#include <QIODevice>
#include <QStringList>
#include <libopenmpt/libopenmpt.h>
#include <libopenmpt/libopenmpt_stream_callbacks_file.h>

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class OpenMPTHelper
{
public:
    explicit OpenMPTHelper(QIODevice *input);
    ~OpenMPTHelper();

    bool initialize();

    static QMap<QString, int> getInterpolators();
    bool isValidInterpolator(int value);
    void setInterpolator(int value);
    bool isValidStereoSeparation(int separation);
    void setStereoSeparation(int separation);

    qint64 read(unsigned char *data, qint64 maxSize);
    void seek(qint64 pos);

    int bitrate() const;
    inline int rate() const { return 44100; }
    inline int channels() const { return 2; }
    inline int depth() const { return 16; }
    inline int length() const { return m_duration; }

    inline const QString title() const { return m_title; }
    inline const QString comment() const { return m_comment; }

    inline int patternCount() const { return m_patternCount; }
    inline int channelCount() const { return m_channelCount; }
    inline int sampleCount() const { return m_sampleCount; }
    inline int instrumentCount() const { return m_instrumentCount; }

    inline const QStringList instruments()const  { return m_instruments; }
    inline const QStringList samples() const { return m_samples; }

private:
    QString toString(const char *input);

    static std::size_t stream_read(void *, void *, std::size_t);
    static int stream_seek(void *, std::int64_t, int);
    static std::int64_t stream_tell(void *);
    static QIODevice *VFS(void *instance) { return reinterpret_cast<QIODevice *>(instance); }

    openmpt_stream_callbacks callbacks = { stream_read, stream_seek, stream_tell };

    QIODevice *m_input = nullptr;
    openmpt_module *m_mod = nullptr;
    int m_duration = 0;
    QString m_title;
    QString m_comment;
    int m_patternCount = 0, m_channelCount = 0;
    int m_instrumentCount = 0, m_sampleCount = 0;
    QStringList m_instruments;
    QStringList m_samples;

};

#endif
