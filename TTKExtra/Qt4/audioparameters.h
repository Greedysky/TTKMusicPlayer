/***************************************************************************
 *   Copyright (C) 2009-2016 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
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

#ifndef AUDIOPARAMETERS_H
#define AUDIOPARAMETERS_H

#include <QtGlobal>
#include "channelmap.h"
#include "qmmp.h"

/*! @brief The AudioParameters class keeps information about audio settings.
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class AudioParameters
{
public:
    /*!
     * Constructor.
     */
    AudioParameters();
    /*!
     * Constructs audio settings with the given parameters.
     * @param srate Sampling rate.
     * @param map Channel map.
     * @param format PCM data format.
     */
    AudioParameters(quint32 srate, const ChannelMap &map, Qmmp::AudioFormat format);
    /*!
     * Constructs a copy of \b other.
     */
    AudioParameters(const AudioParameters &other);
    /*!
     * Assigns audio parameters \b p to this parameters.
     */
    void operator=(const AudioParameters &p);
    /*!
     * Returns \b true if parameters \b p is equal to this parameters; otherwise returns \b false.
     */
    bool operator==(const AudioParameters &p) const;
    /*!
     * Returns \b true if parameters \b p is not equal to this parameters; otherwise returns \b false.
     */
    bool operator!=(const AudioParameters &p) const;
    /*!
     * Returns sample rate in Hz.
     */
    quint32 sampleRate() const;
    /*!
     * Returns number of channels.
     */
    int channels() const;
    /*!
     * Returns channel map.
     */
    const ChannelMap channelMap() const;
    /*!
     * Returns pcm format.
     */
    Qmmp::AudioFormat format() const;
    /*!
     * Returns sample size in bytes.
     */
    int sampleSize() const;
    /*!
     * Returns string represention of the audio parameters.
     * May be useful for debug purposes.
     */
    const QString toString() const;
    /*!
     * Returns sample size in bytes of the given pcm data \b format.
     */
    static int sampleSize(Qmmp::AudioFormat format);

private:
    quint32 m_srate;
    ChannelMap m_chan_map;
    Qmmp::AudioFormat m_format;
    int m_sz;
};

#endif // AUDIOPARAMETERS_H
