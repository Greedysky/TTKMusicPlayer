/***************************************************************************
 *   Copyright (C) 2008-2025 by Ilya Kotov                                 *
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

#ifndef STATEHANDLER_H
#define STATEHANDLER_H

#include <QHash>
#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
#  include <QMutex>
#else
#  include <QRecursiveMutex>
#endif
#include "abstractengine.h"
#include "audioparameters.h"

/*! @brief The StateHandler class allows one to track information about playback progress.
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT StateHandler : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     * @param parent Parent object.
     */
    explicit StateHandler(QObject *parent = nullptr);
    /*!
     * Destructor.
     */
    ~StateHandler();

    /*!
     * Sends information about playback progress.
     * @param elapsed Current time (in milliseconds).
     * @param bitrate Current bitrate (in kbps).
     */
    void dispatch(qint64 elapsed, int bitrate);
    /*!
     * Sends information about audio parameters.
     * @param p Output audio patameters.
     */
    void dispatch(const AudioParameters &p);
    /*!
     * Sends information about song length
     * @param duration track length in milliseconds
     */
    void dispatch(qint64 duration);
    /*!
     * Sends track information.
     * @param info track information.
     */
    bool dispatch(const TrackInfo &info);
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
    qint64 elapsed() const;
    /*!
     * Returns duration in milliseconds
     */
    qint64 duration() const;
    /*!
     * Returns current bitrate (in kbps)
     */
    int bitrate() const;
    /*!
     * Returns output audio parameters.
     */
    AudioParameters audioParameters() const;
    /*!
     * Returns the current state.
     */
    Qmmp::State state() const;
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
     * Emitted when audio parameters have changed.
     * @param p New audio parameters for output.
     */
    void audioParametersChanged(const AudioParameters &p);
     /*!
     * This signal is emitted when the stream reader fills it's buffer.
     * The argument \b progress indicates the current percentage of buffering completed.
     */
    void bufferingProgress(int progress);

private:
    qint64 m_elapsed = -1;
    qint64 m_duration = 0;
    bool m_sendAboutToFinish = true;
    int m_bitrate = 0;
    static StateHandler* m_instance;
    TrackInfo m_info;
    QHash<QString, QString> m_streamInfo;
    Qmmp::State m_state = Qmmp::Stopped;
    AudioParameters m_audioParameters;
#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
    mutable QMutex m_mutex;
#else
    mutable QRecursiveMutex m_mutex;
#endif
};

#endif
