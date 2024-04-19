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

#ifndef OUTPUT_H
#define OUTPUT_H

#include <QList>
#include <QIODevice>
#include "outputfactory.h"
#include "audioparameters.h"
#include "trackinfo.h"
#include "channelmap.h"

class QTimer;
class QmmpSettings;
class QmmpPluginCache;

/*! @brief The Output class provides the base interface class of audio outputs.
 * @author Brad Hughes <bhughes@trolltech.com>
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT Output
{
public:
    /*!
     * Object contsructor.
     */
    Output() = default;
    /*!
     * Destructor.
     */
    virtual ~Output() = default;

    /*!
     * Prepares object for usage and setups required audio parameters.
     * Subclass should reimplement this function.
     * @param freq Sample rate.
     * @param map Map of channels.
     * @param format Audio format
     * @return initialization result (\b true - success, \b false - failure)
     */
    virtual bool initialize(quint32 freq, ChannelMap map, Qmmp::AudioFormat format) = 0;
    /*!
     * Returns output interface latency in milliseconds.
     */
    virtual qint64 latency() = 0;
    /*!
     * Requests playback to pause. If it was paused already, playback should resume.
     * Subclasses that reimplement this function must call the base implementation.
     */
    /*!
     * Writes up to \b maxSize bytes from \b data to the output interface device.
     * Returns the number of bytes written, or -1 if an error occurred.
     * Subclass should reimplement this function.
     */
    virtual qint64 writeAudio(unsigned char *data, qint64 maxSize) = 0;
    /*!
     * Writes all remaining plugin's internal data to audio output device.
     * Subclass should reimplement this function.
     */
    virtual void drain() = 0;
    /*!
     * Drops all plugin's internal data, resets audio device
     * Subclass should reimplement this function.
     */
    virtual void reset() = 0;
    /*!
     * Stops processing audio data, preserving buffered audio data.
     */
    virtual void suspend();
    /*!
     * Resumes processing audio data.
     */
    virtual void resume();
    /*!
     * Sets track information for output.
     * Default implementation does nothing.
     * Reimplement this function to receive metadata.
     */
    virtual void setTrackInfo(const TrackInfo &info);
    /*!
     * Returns selected audio parameters.
     */
    AudioParameters audioParameters() const;
    /*!
     * Returns samplerate.
     */
    quint32 sampleRate() const;
    /*!
     * Returns number of channels.
     */
    int channels() const;
    /*!
     * Returns required channel map.
     */
    const ChannelMap &channelMap() const;
    /*!
     * Returns selected audio format.
     */
    Qmmp::AudioFormat format() const;
    /*!
     * Returns sample size in bytes.
     */
    int sampleSize() const;
    /*!
     * Creates selected output.
     * @return Output subclass object.
     */
    static Output *create();
    /*!
     * Returns a list of output factories.
     */
    static QList<OutputFactory*> factories();
    /*!
     * Returns plugin file path.
     * @param factory Output plugin factory.
     */
    static QString file(const OutputFactory *factory);
    /*!
     * Selects current output \b factory.
     */
    static void setCurrentFactory(const OutputFactory* factory);
    /*!
     * Returns selected output factory.
     */
    static OutputFactory *currentFactory();

protected:
    /*!
     * Use this function inside initialize() reimplementation to tell about accepted audio parameters.
     * @param freq Sample rate.
     * @param map Map of channels.
     * @param format Audio format.
     */
    void configure(quint32 freq, ChannelMap map, Qmmp::AudioFormat format);

private:
    quint32 m_frequency = 0;
    ChannelMap m_chan_map;
    Qmmp::AudioFormat m_format = Qmmp::PCM_UNKNOWN;
    int m_sample_size = 0;

    static void loadPlugins();
    static QList<QmmpPluginCache*> *m_cache;

};


#endif
