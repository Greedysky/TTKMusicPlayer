/***************************************************************************
 *   Copyright (C) 2009-2024 by Ilya Kotov                                 *
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

#ifndef AUDIOPARAMETERS_H
#define AUDIOPARAMETERS_H

#include "channelmap.h"

/*! @brief The AudioParameters class keeps information about audio settings.
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT AudioParameters
{
public:
    /*!
     * Byte order of samples.
     */
    enum ByteOrder
    {
        LittleEndian = 0, /*!< Samples are in little-endian byte order */
        BigEndian         /*!< Samples are in big-endian byte order */
    };
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
    AudioParameters &operator=(const AudioParameters &p);
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
     * Returns the number of bytes required to represent one frame
     * (a sample in each channel) in this format.
     */
    int frameSize() const;
    /*!
     * Returns sample size in bits.
     */
    int bitsPerSample() const;
    /*!
     * Returns the number of used bits in the sample. The value shoud be
     * less or equal to the value returned by \b bitsPerSample().
     */
    int validBitsPerSample() const;
    /*!
     * Returns byte order for selected audio format.
     */
    ByteOrder byteOrder() const;
    /*!
     * Returns string represention of the audio parameters.
     * May be useful for debug purposes.
     */
    const QString toString() const;
    /*!
     * Returns sample size in bytes of the given pcm data \b format.
     */
    static int sampleSize(Qmmp::AudioFormat format);
    /*!
     * Returns sample size in bits of the given pcm data \b format.
     */
    static int bitsPerSample(Qmmp::AudioFormat format);
    /*!
     * Returns the number of used bits in the sample
     * of the given pcm data \b format.
     */
    static int validBitsPerSample(Qmmp::AudioFormat format);
    /*!
     * Find audio format by number of bits and byte  order.
     * Returns \b Qmmp::UNKNOWN if format is not found.
     * @param bits Number of used bits in the sample (precision).
     * @param byteOrder Byte order.
     */
    static Qmmp::AudioFormat findAudioFormat(int bits, ByteOrder byteOrder = LittleEndian);

private:
    quint32 m_srate = 0;
    ChannelMap m_chan_map;
    Qmmp::AudioFormat m_format = Qmmp::PCM_S16LE;
    int m_sz = 2;
    int m_precision = 16;

};

#endif
