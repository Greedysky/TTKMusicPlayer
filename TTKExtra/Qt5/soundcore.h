/***************************************************************************
 *   Copyright (C) 2006-2013 by Ilya Kotov                                 *
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
#ifndef SOUNDCORE_H
#define SOUNDCORE_H

#include <QObject>
#include <QString>
#include <QQueue>
#include <QHash>
#include "decoder.h"
#include "output.h"
#include "visual.h"
#include "qmmp.h"
#include "qmmpsettings.h"
#include "eqsettings.h"

class VolumeControl;
class AbstractEngine;
class InputSource;
class StateHandler;

/*! \brief The SoundCore class provides a simple interface for audio playback.
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class Q_DECL_EXPORT SoundCore : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     * @param parent Parent object.
     */
    SoundCore(QObject *parent = 0);
    /*!
     * Destructor.
     */
    ~SoundCore();
    /*!
     * Returns length in milliseconds
     */
    qint64 totalTime() const;
    /*!
     * Returns equalizer settings
     */
    EqSettings eqSettings() const;
    /*!
     * Chages equalizer settings to \b settings.
     */
    void setEqSettings(const EqSettings &settings);
    /*!
     * Returns left volume level.
     */
    int leftVolume() const;
    /*!
     * Returns left volume level.
     */
    int rightVolume() const;
    /*!
     * Returns the highest volume of the left and right channels.
     */
    int volume() const;
    /*!
     * Returns the balance between left and right channels.
     */
    int balance() const;
    /*!
     * Returns \b true if volume is unmuted, otherwise returns \b false
     */
    bool isMuted() const;
    /*!
     * Returns the current time (in milliseconds).
     */
    qint64 elapsed();
    /*!
     * Returns current bitrate (in kbps)
     */
    int bitrate();
    /*!
     * Returns current sample rate (in Hz).
     */
    quint32 frequency();
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
    Qmmp::State state() const;
    /*!
     * Returns all meta data in map.
     */
    QMap <Qmmp::MetaData, QString> metaData() const;
    /*!
     * Returns the metdata string associated with the given \b key.
     */
    QString metaData(Qmmp::MetaData key) const;
    /*!
     * Returns a hash of stream information if available
     */
    QHash<QString, QString> streamInfo() const;
    /*!
     *  Indicates that the current active engine will be used for the next queued track.
     *  May be useful for some effect plugins.
     */
    bool nextTrackAccepted() const;
    /*!
     * Returns a pointer to the SoundCore instance.
     */
    static SoundCore* instance();

public slots:
    /*!
     * Sets volume.
     * @param left - volume of the left channel \b [0..100].
     * @param right - volume of the right channel \b [0..100].
     */
    void setVolume(int left, int right);
    /*!
     * Mutes/Restores volume
     * @param mute - state of volume (\b true - mute, \b false - restore)
     */
    void setMuted(bool mute);
    /*!
     * Changes volume by \b delta percent
     */
    void changeVolume(int delta);
    /*!
     * Sets the volume of the left and right channels with keeping of the balance.
     * @param volume volume of the left and right channels \b [0..100].
     */
    void setVolume(int volume);
    /*!
     * Sets the balance between left and right channels.
     * @param balance balance between left and right channels \b [-100..100].
     */
    void setBalance(int balance);
    /*!
     * This function plays file or stream with the given path \p source.
     * Returns \b true if playback has been started successful or source is not a local file,
     * otherwise returns \b false. Useful for invalid files skipping.
     */
    bool play(const QString &source, bool queue = false, qint64 offset = -1);
    /*!
     *  Stops playback
     */
    void stop();
    /*!
     *  Pauses/resumes playback
     */
    void pause();
    /*!
     *  This function sets the current play position to \p pos in milliseconds.
     */
    void seek(qint64 pos);
    /*!
     *  This function returns file path or stream url.
     */
    const QString url() const;

signals:
    /*!
     * This signal is emitted when the stream reader fills it's buffer.
     * The argument \b progress indicates the current percentage of buffering completed.
     */
    void bufferingProgress(int progress);
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
     * Emitted when new metadata is available.
     */
    void metaDataChanged ();
    /*!
     * Emitted when new stream information is available.
     */
    void streamInfoChanged();
    /*!
     * This signal is emitted when the state of the SoundCore has changed.
     */
    void stateChanged (Qmmp::State newState);
    /*!
     * Emitted when playback has finished.
     */
    void finished();
    /*!
     * Emitted when volume has changed.
     * @param left Left channel volume level. It should be \b [0..100]
     * @param right Right channel volume level. It should be \b [0..100]
     */
    void volumeChanged(int left, int right);
    /*!
     * Emitted when volume has muted or restored
     * @param muted new state of the volume (\b true - muted, \b false - unmuted)
     */
    void mutedChanged(bool muted);
    /*!
     * Emitted when the highest volume of the left and right channels has changed.
     * @param volume new value of the highest volume of the left and right channels.
     */
    void volumeChanged(int volume);
    /*!
     * Emitted when the balance between left and right channels has changed.
     * @param balance new balance value.
     */
    void balanceChanged(int balance);
    /*!
     * Emitted when equalizer settings has changed.
     */
    void eqSettingsChanged();
    /*!
     * Emitted before playback ends. Use this signal to append new url to the queue.
     */
    void nextTrackRequest();

private slots:
    void startNextSource();
    void startNextEngine();

private:
    bool event(QEvent *e);
    enum NextEngineState
    {
        NO_ENGINE = 0,
        SAME_ENGINE,
        ANOTHER_ENGINE,
        INVALID_SOURCE
    };
    QMap <Qmmp::MetaData, QString> m_metaData;
    QHash <QString, QString> m_streamInfo;
    QString m_url;
    static SoundCore* m_instance;
    StateHandler *m_handler;
    VolumeControl *m_volumeControl;
    AbstractEngine *m_engine;
    QQueue<InputSource *> m_sources;
    int m_nextState;
    bool m_muted;
};

#endif
