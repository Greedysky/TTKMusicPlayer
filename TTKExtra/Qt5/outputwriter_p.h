/***************************************************************************
 *   Copyright (C) 2012-2014 by Ilya Kotov                                 *
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

#ifndef OUTPUTWRITER_P_H
#define OUTPUTWRITER_P_H

#include <QThread>
#include <QMutex>
#include "recycler_p.h"
#include "audioparameters.h"
#include "channelmap.h"

class QTimer;
class QmmpSettings;
class StateHandler;
class Output;
class Effect;

/** @internal
    @brief Output thread.
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class Q_DECL_EXPORT OutputWriter : public QThread
{
    Q_OBJECT
public:
    explicit OutputWriter(QObject *parent = 0);

    virtual ~OutputWriter();
    /*!
     * Prepares object for usage and setups required audio parameters.
     * @param freq Sample rate.
     * @param map Map of channels.
     * @param format Audio format
     * @return initialization result (\b true - success, \b false - failure)
     */
    bool initialize(quint32 freq, ChannelMap map, Qmmp::AudioFormat format);
    /*!
     * Requests playback to pause. If it was paused already, playback should resume.
     */
    void pause();
    /*!
     * Requests playback to stop.
     */
    void stop();
    /*!
     * Mutes/Restores volume
     * @param mute state of volume (\b true - mute, \b false - restore)
     */
    void setMuted(bool muted);
    /*!
     * Requests playback to finish.
     */
    void finish();
    /*!
     * Requests a seek to the time \b pos indicated, specified in milliseconds.
     * If \b reset is \b true, this function also clears internal output buffers for faster seeking;
     * otherwise does nothing with buffers.
     */
    void seek(qint64 pos, bool reset = false);
    /*!
     * Returns Recycler pointer.
     */
    Recycler *recycler();
    /*!
     * Returns mutex pointer.
     */
    QMutex *mutex();
    /*!
     * Returns selected audio parameters.
     */
    AudioParameters audioParameters() const;
    /*!
     * Returns samplerate.
     */
    quint32 sampleRate();
    /*!
     * Returns channels number.
     */
    int channels();
    /*!
     * Returns selected audio format.
     */
    Qmmp::AudioFormat format() const;
    const ChannelMap channelMap() const;
    /*!
     * Returns sample size in bytes.
     */
    int sampleSize() const;
    void updateEqSettings();

private:
    void run(); //thread run function
    void status();
    void dispatch(qint64 elapsed,
                  int bitrate,
                  int frequency,
                  int bits,
                  int channels);
    void dispatch(const Qmmp::State &state);
    void dispatchVisual(Buffer *buffer);
    void applyConverters(Buffer *buffer);
    void clearVisuals();
    bool prepareConverters();

    bool m_skip;
    QMutex m_mutex;
    Recycler m_recycler;
    StateHandler *m_handler;
    quint32 m_frequency;
    int m_channels, m_kbps;
    ChannelMap m_chan_map;
    Qmmp::AudioFormat m_format;
    qint64 m_bytesPerMillisecond;
    bool m_user_stop, m_pause;
    bool m_prev_pause;
    bool m_finish;
    bool m_useEq;
    qint64 m_totalWritten, m_currentMilliseconds;
    unsigned char *m_visBuffer;
    qint64 m_visBufferSize;
    QmmpSettings *m_settings;
    Output *m_output;
    bool m_muted;
    QList<Effect *> m_converters;

};

#endif // OUTPUTWRITER_P_H
