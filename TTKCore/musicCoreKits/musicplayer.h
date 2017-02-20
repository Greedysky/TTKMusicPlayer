#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QTimer>
#include "musicobject.h"
#include "musicglobaldefine.h"

#ifdef Q_CC_GNU
#  pragma GCC diagnostic ignored "-Wswitch"
#endif

class SoundCore;
class MusicPlaylist;

/*! @brief The class of the music player.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicPlayer : public QObject
{
    Q_OBJECT
public:
    enum State
    {
        StoppedState,   ///*stop state*/
        PlayingState,   ///*play state*/
        PausedState     ///*pause state*/
    };

    enum Enhanced
    {
        EnhancedOff,    ///*enhanced off, no any effect*/
        Music3D,        ///*3d music effect*/
        MusicNICAM,     ///*nicam music effect*/
        MusicSubwoofer, ///*subwoofer music effect*/
        MusicVocal      ///*vocal music effect*/
    };

    explicit MusicPlayer(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicPlayer();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    State state() const;
    /*!
     * Get current player state.
     */
    void setPlaylist(MusicPlaylist *playlist);
    /*!
     * Set current play list.
     */
    MusicPlaylist *playlist() const;
    /*!
     * Get current play list.
     */

    qint64 duration() const;
    /*!
     * Get current player duration.
     */
    qint64 position() const;
    /*!
     * Get current player postion.
     */

    void playNext();
    /*!
     * Set current player to next music.
     */
    void playPrevious();
    /*!
     * Set current player to previous music.
     */

    int volume() const;
    /*!
     * Get current player volume.
     */
    void setVolume(int volume);
    /*!
     * Set current player volume.
     */
    bool isMuted() const;
    /*!
     * Get current player mute state.
     */
    void setMuted(bool muted);
    /*!
     * Set current player mute or not.
     */
    void setPosition(qint64 position);
    /*!
     * Set current play pos.
     */

    void setMusicEnhanced(Enhanced type);
    /*!
     * Set current music enhanced effect.
     */
    Enhanced getMusicEnhanced() const;
    /*!
     * Get current music enhanced effect.
     */

Q_SIGNALS:
    void stateChanged(MusicPlayer::State newState);
    /*!
     * Current state changed.
     */
    void durationChanged(qint64 duration);
    /*!
     * Current duration changed.
     */
    void positionChanged(qint64 position);
    /*!
     * Current position changed.
     */
    void volumeChanged(int volume);
    /*!
     * Current volume changed.
     */
    void mutedChanged(bool muted);
    /*!
     * Current mute state changed.
     */

public Q_SLOTS:
    void play();
    /*!
     * Player to play.
     */
    void pause();
    /*!
     * Player to pause.
     */
    void stop();
    /*!
     * Player to stop.
     */
    void setEqEffect(const MusicObject::MIntList &hz);
    /*!
     * Set current equalizer effect.
     */
    void setEnaleEffect(bool enable);
    /*!
     * Set enable to set current equalizer effect.
     */
    void setEqInformation();
    /*!
     * Read equalizer effect from config file.
     */
    void setSoundEffectVolume(int value);
    /*!
     * Set current player volume.
     */
    void removeCurrentMedia();
    /*!
     * Remove current music media data.
     */

private Q_SLOTS:
    void update();
    /*!
     * Player one second time out.
     */
    void getCurrentDuration();
    /*!
     * Get current duration by time out.
     */

protected:
    void setMusicEnhancedCase();
    /*!
     * Set current music enhanced effect option.
     */

    MusicPlaylist *m_playlist;
    State m_state;
    SoundCore *m_music;
    QTimer m_timer;
    QString m_currentMedia;
    Enhanced m_musicEnhanced;
    qint64 m_duration;

    int m_tryTimes;
    int m_volumeMusic3D, m_balance;
    float m_posOnCircle;

};

#endif // MUSICPLAYER_H
