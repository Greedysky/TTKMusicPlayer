#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

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

#include <QTimer>
#include "musicglobaldefine.h"

class SoundCore;
class MusicPlaylist;

/*! @brief The namespace of the global defines.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    enum class Enhance
    {
        Off,        /*!< enhance off, no any effect */
        M3D,        /*!< 3d media effect */
        NICAM,      /*!< nicam media effect */
        Subwoofer,  /*!< subwoofer media effect */
        Vocal       /*!< vocal media effect */
    };
}

/*! @brief The class of the music player.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPlayer : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicPlayer)
public:
    /*!
     * Object constructor.
     */
    explicit MusicPlayer(QObject *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicPlayer();

    /*!
     * Get current player is playing or not.
     */
    bool isPlaying() const noexcept;
    /*!
     * Get current player state.
     */
    TTK::PlayState state() const noexcept;

    /*!
     * Set current play list.
     */
    void setPlaylist(MusicPlaylist *playlist) noexcept;

    /*!
     * Get current player duration.
     */
    qint64 duration() const;
    /*!
     * Get current player position.
     */
    qint64 position() const;
    /*!
     * Set current play pos.
     */
    void setPosition(qint64 position);

    /*!
     * Get current player volume.
     */
    int volume() const;
    /*!
     * Set current player volume.
     */
    void setVolume(int volume);
    /*!
     * Get current player mute state.
     */
    bool isMuted() const noexcept;
    /*!
     * Set current player mute or not.
     */
    void setMuted(bool muted);

    /*!
     * Set current media enhance effect.
     */
    void setEnhance(TTK::Enhance type);
    /*!
     * Get current media enhance effect.
     */
    TTK::Enhance enhance() const noexcept;

Q_SIGNALS:
    /*!
     * Current state changed.
     */
    void stateChanged(TTK::PlayState state);
    /*!
     * Current duration changed.
     */
    void durationChanged(qint64 duration);
    /*!
     * Current position changed.
     */
    void positionChanged(qint64 position);

public Q_SLOTS:
    /*!
     * Player to play.
     */
    void play();
    /*!
     * Player to pause.
     */
    void pause();
    /*!
     * Player to stop.
     */
    void stop();
    /*!
     * Set current equalizer effect.
     */
    void setEqualizerEffect(const TTKIntList &hz);
    /*!
     * Set enable to set current equalizer effect.
     */
    void setEnabledEffect(bool enabled);
    /*!
     * Read equalizer effect from config file.
     */
    void setEqualizerConfig();

private Q_SLOTS:
    /*!
     * Player one second time out.
     */
    void update();
    /*!
     * Generate current duration by time out.
     */
    void generateDuration();

private:
    /*!
     * set current play state to stop.
     */
    void setStopState();
    /*!
     * set current play state.
     */
    void setCurrentPlayState(TTK::PlayState state) noexcept;

    MusicPlaylist *m_playlist;
    TTK::PlayState m_state;
    SoundCore *m_core;
    QTimer m_timer;
    QString m_currentMedia;
    qint64 m_duration;
    int m_durationTimes;

    struct Enhance
    {
        TTK::Enhance m_type = TTK::Enhance::Off;
        int m_volume3D = 0;
        float m_circlePos = 0;

    } m_enhance;

};

#endif // MUSICPLAYER_H
