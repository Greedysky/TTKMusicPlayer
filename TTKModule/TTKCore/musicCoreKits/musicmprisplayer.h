#ifndef MUSICMPRISPLAYER_H
#define MUSICMPRISPLAYER_H

/***************************************************************************
 * This file is part of the TTK Music Player project
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

#include "musicglobaldefine.h"

#include <QDBusConnection>
#include <QDBusObjectPath>
#include <QDBusAbstractAdaptor>

class SoundCore;
class MusicApplication;
class MusicMPRISPlayerRoot;
class MusicMPRISPlayerCore;

/*! @brief The class of the app mpris main widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicMPRISPlayer : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    MusicMPRISPlayer(QObject *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicMPRISPlayer();

    /*!
     * Execute module.
     */
    void execute();

private:
    MusicMPRISPlayerRoot *m_root;
    MusicMPRISPlayerCore *m_player;

};


/*! @brief The class of the app mpris root object.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicMPRISPlayerRoot : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2")
    Q_PROPERTY(bool CanQuit READ canQuit)
    Q_PROPERTY(bool CanRaise READ canRaise)
    Q_PROPERTY(QString DesktopEntry READ desktopEntry)
    Q_PROPERTY(bool HasTrackList READ hasTrackList)
    Q_PROPERTY(QString Identity READ identity)
    Q_PROPERTY(QStringList SupportedMimeTypes READ supportedMimeTypes)
    Q_PROPERTY(QStringList SupportedUriSchemes READ supportedUriSchemes)

public:
    /*!
     * Object constructor.
     */
    MusicMPRISPlayerRoot(QObject *parent = nullptr);

    bool canQuit() const;
    bool canRaise() const;
    QString desktopEntry() const;
    bool hasTrackList() const;
    QString identity() const;
    QStringList supportedMimeTypes() const;
    QStringList supportedUriSchemes() const;

public Q_SLOTS:
    void Quit();
    void Raise();

};


/*! @brief The class of the app mpris player object.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicMPRISPlayerCore : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2.Player")
    Q_PROPERTY(bool CanControl READ canControl)
    Q_PROPERTY(bool CanGoNext READ canGoNext)
    Q_PROPERTY(bool CanGoPrevious READ canGoPrevious)
    Q_PROPERTY(bool CanPause READ canPause)
    Q_PROPERTY(bool CanPlay READ canPlay)
    Q_PROPERTY(bool CanSeek READ canSeek)
    Q_PROPERTY(QString LoopStatus READ loopStatus WRITE setLoopStatus)
    Q_PROPERTY(double MaximumRate READ maximumRate)
    Q_PROPERTY(QVariantMap Metadata READ metadata)
    Q_PROPERTY(double MinimumRate READ minimumRate)
    Q_PROPERTY(QString PlaybackStatus READ playbackStatus)
    Q_PROPERTY(qlonglong Position READ position)
    Q_PROPERTY(double Rate READ rate WRITE setRate)
    Q_PROPERTY(bool Shuffle READ shuffle WRITE setShuffle)
    Q_PROPERTY(double Volume READ volume WRITE setVolume)

public:
    /*!
     * Object constructor.
     */
    MusicMPRISPlayerCore(QObject *parent = nullptr);

    bool canControl() const noexcept;
    bool canGoNext() const noexcept;
    bool canGoPrevious() const noexcept;
    bool canPause() const noexcept;
    bool canPlay() const noexcept;
    bool canSeek() const noexcept;
    QString loopStatus() const noexcept;
    void setLoopStatus(const QString &value) noexcept;
    double maximumRate() const noexcept;
    QVariantMap metadata() const;
    double minimumRate() const noexcept;
    QString playbackStatus() const noexcept;
    qlonglong position() const;
    double rate() const noexcept;
    void setRate(double value) noexcept;
    bool shuffle() const noexcept;
    void setShuffle(bool value) noexcept;
    double volume() const;
    void setVolume(double value);

public Q_SLOTS:
    void Next();
    void Previous();
    void OpenUri(const QString &uri);
    void Pause();
    void Play();
    void PlayPause();
    void Stop();
    void Seek(qlonglong offset);
    void SetPosition(const QDBusObjectPath &trackId, qlonglong position);

Q_SIGNALS:
    void Seeked(qlonglong position);

private Q_SLOTS:
    /*!
     * When new track information is available.
     */
    void trackInfoChanged();
    /*!
     * When the state of the SoundCore has changed.
     */
    void stateChanged();
    /*!
     * When the highest volume of the left and right channels has changed.
     */
    void volumeChanged();
    /*!
     * Tracks elapesed time.
     */
    void elapsedChanged(qint64 elapsed);
    /*!
     * Current play mode changed.
     */
    void playbackModeChanged();

private:
    void updateTrackID();
    void syncProperties();
    void sendProperties();

    int m_prevTrack;
    qint64 m_prevPos;
    SoundCore *m_core;
    MusicApplication *m_application;
    QDBusObjectPath m_trackID;
    TTKVariantMap m_properties;

};

#endif // MUSICMPRISPLAYER_H
