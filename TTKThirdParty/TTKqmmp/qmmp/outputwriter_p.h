/***************************************************************************
 *   Copyright (C) 2012-2026 by Ilya Kotov                                 *
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

#ifndef OUTPUTWRITER_P_H
#define OUTPUTWRITER_P_H

#include <QThread>
#include <QMutex>
#include <atomic>
#include "recycler_p.h"
#include "audioparameters.h"
#include "channelmap.h"

class QTimer;
class QmmpSettings;
class StateHandler;
class Output;
class Effect;
class AudioConverter;
class ChannelConverter;

/** @internal
    @brief Output thread.
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class QMMP_EXPORT OutputWriter : public QThread
{
    Q_OBJECT
public:
    explicit OutputWriter(QObject *parent = nullptr);

    virtual ~OutputWriter();

    /*!
     * Prepares object for usage and setups required audio parameters.
     * @param freq Sample rate.
     * @param map Map of channels.
     * @return initialization result (\b true - success, \b false - failure)
     */
    bool initialize(quint32 freq, ChannelMap map);
    /*!
     * Requests playback to pause. If it was paused already, playback should resume.
     */
    void pause();
    /*!
     * Requests playback to stop.
     */
    void stop();
    /*!
     * Requests playback to finish.
     */
    void finish();
    /*!
     * Requests a seek to the time \b pos indicated, specified in milliseconds.
     * If \b reset is \b true, this function also clears internal output buffers for faster seeking;
     * otherwise does nothing with buffers.
     */
    void seek(qint64 time, bool reset = false);
    /*!
     * Returns Recycler pointer.
     */
    Recycler *recycler();
    /*!
     * Returns selected audio parameters, i.e. passed by \b initialize function.
     */
    const AudioParameters &inputAudioParameters() const;
    /*!
     * Returns output audio parameters, i.e. accepted by output device.
     */
    AudioParameters outputAudioParameters() const;
    /*!
     * Returns sample size in bytes.
     */
    int sampleSize() const;
    /*!
     * Update eq settings.
     */
    void updateEqSettings();

private:
    virtual void run() override final; //thread run function

    void status();
    void dispatch(qint64 elapsed, int bitrate);
    void dispatch(const Qmmp::State &state);
    void dispatch(const AudioParameters &p);
    void dispatchVisual(Buffer *buffer);
    bool prepareConverters();
    void startVisualization();
    void stopVisualization();

    bool m_skip = false;
    QMutex m_mutex;
    Recycler m_recycler;
    StateHandler *m_handler;
    quint32 m_frequency = 0;
    int m_channels = 0, m_kbps = 0;
    ChannelMap m_chan_map;
    Qmmp::AudioFormat m_format = Qmmp::PCM_UNKNOWN;
    qint64 m_bytesPerMillisecond = 0;
    std::atomic_bool m_user_stop = ATOMIC_VAR_INIT(false);
    std::atomic_bool m_pause = ATOMIC_VAR_INIT(false);
    std::atomic_bool m_muted = ATOMIC_VAR_INIT(false);
    std::atomic_bool m_finish = ATOMIC_VAR_INIT(false);
    bool m_paused = false;
    bool m_useEq = false;
    bool m_abr = false;
    qint64 m_totalWritten = 0, m_currentMilliseconds = -1;
    QmmpSettings *m_settings = nullptr;
    Output *m_output = nullptr;
    AudioParameters m_in_params;
    AudioConverter *m_format_converter = nullptr;
    ChannelConverter *m_channel_converter = nullptr;
    unsigned char *m_output_buf = nullptr;
    size_t m_output_size = 0; //samples

};

#endif
