/***************************************************************************
 *   Copyright (C) 2008-2012 by Ilya Kotov                                 *
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
#ifndef STATEHANDLER_H
#define STATEHANDLER_H

#include <QObject>
#include <QMap>
#include <QHash>
#include <QMutex>
#include "abstractengine.h"
#include "qmmp.h"

/*! @brief The StateHandler class allows one to track information about playback progress.
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class Q_DECL_EXPORT StateHandler : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     * @param parent Parent object.
     */
    StateHandler(QObject *parent = 0);
    /*!
     * Destructor.
     */
    ~StateHandler();
    /*!
     * Sends information about playback progress.
     * @param elapsed Current time (in milliseconds).
     * @param bitrate Current bitrate (in kbps).
     * @param frequency Current samplerate (in Hz).
     * @param precision Sample size (in bits).
     * @param channels Number of channels.
     */
    void dispatch(qint64 elapsed, int bitrate, quint32 frequency, int precision, int channels);
    /*!
     * Sends information about song length
     * @param length song length in milliseconds
     */
    void dispatch(qint64 length);
    /*!
     * Sends metadata \b metaData
     */
    void dispatch(const QMap<Qmmp::MetaData, QString> &metaData);
    /*!
     * Sends stream information \b info
     */
    void dispatch(const QHash<QString, QString> &info);
    /*!
     * Sends playback state.
     */
    void dispatch(Qmmp::State state);
    /*!
     * Sends buffering progress.
     * @param percent Indicates the current percentage of buffering completed.
     */
    void dispatchBuffer(int percent);
    /*!
     * Returns the current time (in milliseconds).
     */
    qint64 elapsed();
    /*!
     * Returns length in milliseconds
     */
    qint64 totalTime();
    /*!
     * Returns current bitrate (in kbps)
     */
    int bitrate();
    /*!
     * Returns current sample rate (in Hz).
     */
    int frequency();
    /*!
     * Returns sample size (in bits).
     */
    int sampleSize();
    /*!
     * Returns channels number.
     */
    int channels();
    /*!
     * Returns the current state.
     */
    Qmmp::State state();
    /*!
     * Sends next track request.
     */
    void sendNextTrackRequest();
    /*!
     * Sends playback finished event.
     */
    void sendFinished();
    /*!
     * Returns a pointer to the first created StateHandler instance.
     */
    static StateHandler* instance();

signals:
    /*!
     * Tracks elapesed time.
     * @param time New track position in milliseconds.
     */
    void elapsedChanged(qint64 time);
    /*!
     * Emitted when bitrate has changed.
     * @param bitrate New bitrate (in kbps)
     */
    void bitrateChanged(int bitrate);
    /*!
     * Emitted when samplerate has changed.
     * @param frequency New sample rate (in Hz)
     */
    void frequencyChanged(quint32 frequency);
    /*!
     * Emitted when sample size has changed.
     * @param size New sample size (in bits)
     */
    void sampleSizeChanged(int size);
    /*!
     * Emitted when channels number has changed.
     * @param channels New channels number.
     */
    void channelsChanged(int channels);
     /*!
     * This signal is emitted when the stream reader fills it's buffer.
     * The argument \b progress indicates the current percentage of buffering completed.
     */
    void bufferingProgress(int progress);


private:
    qint64 m_elapsed;
    qint64 m_length;
    quint32 m_frequency;
    bool m_sendAboutToFinish;
    int m_bitrate, m_precision, m_channels;
    static StateHandler* m_instance;
    QMap <Qmmp::MetaData, QString> m_metaData;
    QHash <QString, QString> m_streamInfo;
    Qmmp::State m_state;
    QMutex m_mutex;
};

#endif
