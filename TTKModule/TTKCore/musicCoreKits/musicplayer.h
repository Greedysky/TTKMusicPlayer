#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

/*! @brief The class of the music player.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPlayer : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicPlayer)
public:
    enum class Enhance
    {
        Off,        /*!< enhance off, no any effect*/
        M3D,        /*!< 3d music effect*/
        NICAM,      /*!< nicam music effect*/
        Subwoofer,  /*!< subwoofer music effect*/
        Vocal       /*!< vocal music effect*/
    };

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
    bool isPlaying() const;
    /*!
     * Get current player state.
     */
    TTK::PlayState state() const;
    /*!
     * Set current play list.
     */
    void setPlaylist(MusicPlaylist *playlist);
    /*!
     * Get current play list.
     */
    MusicPlaylist *playlist() const;

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
    bool isMuted() const;
    /*!
     * Set current player mute or not.
     */
    void setMuted(bool muted);

    /*!
     * Set current music enhanced effect.
     */
    void setEnhanced(Enhance type);
    /*!
     * Get current music enhanced effect.
     */
    Enhance enhanced() const;

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
    void setCurrentPlayState(TTK::PlayState state);

    MusicPlaylist *m_playlist;
    TTK::PlayState m_state;
    SoundCore *m_core;
    QTimer m_timer;
    QString m_currentMedia;
    Enhance m_enhance;
    qint64 m_duration;

    int m_durationTimes;
    int m_volumeMusic3D;
    float m_posOnCircle;

};

#endif // MUSICPLAYER_H
