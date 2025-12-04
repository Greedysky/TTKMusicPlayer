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

#ifndef ADLMIDIHELPER_H
#define ADLMIDIHELPER_H

#include <QFile>
#include <qmmp/qmmp.h>
#include <adlmidi.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class AdlMidiHelper
{
public:
    explicit AdlMidiHelper(const QString &path);
    ~AdlMidiHelper();

    void deinit();
    bool initialize();

    void readSettings();

    inline void seek(qint64 time) { adl_positionSeek(m_input, time / 1000.0); }
    inline qint64 totalTime() const { return adl_totalTimeLength(m_input) * 1000; }

    inline int bitrate() const { return 8; }
    inline int sampleRate() const { return 44100; }
    inline int channels() const { return 2; }
    inline int depth() const { return 16; }

    qint64 read(unsigned char *data, qint64 maxSize);

    QString trackTitle() const;
    QString metaMarker() const;
    inline int trackTitleCount() const { return adl_metaTrackTitleCount(m_input); }
    inline int metaMarkerCount() const { return adl_metaMarkerCount(m_input); }
    inline QString title() const { return adl_metaMusicTitle(m_input); }
    inline QString copyright() const { return adl_metaMusicCopyright(m_input); }

private:
    QString m_path;
    ADL_MIDIPlayer *m_input = nullptr;

};

#endif
