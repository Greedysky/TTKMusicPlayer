#ifndef MUSICCOREMPLAYER_H
#define MUSICCOREMPLAYER_H

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

class QProcess;

/*! @brief The class of the mplayer core.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicCoreMPlayer : public QObject
{
    Q_OBJECT
public:
    enum Category
    {
        NullCategory,   /*!< no category*/
        MusicCategory,  /*!< music category*/
        RadioCategory,  /*!< radio category*/
        VideoCategory   /*!< video category*/
    };

    /*!
     * Object contsructor.
     */
    explicit MusicCoreMPlayer(QObject *parent = 0);

    ~MusicCoreMPlayer();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set media by type and data path.
     */
    void setMedia(Category type, const QString &data, int winId = -1);
    /*!
     * Set current play pos.
     */
    void setPosition(qint64 pos);
    /*!
     * Set left player volume mode.
     */
    void setLeftVolume();
    /*!
     * Set right player volume mode.
     */
    void setRightVolume();
    /*!
     * Set single voice from multi voice by id.
     */
    void setMultiVoice(int number);

    /*!
     * Set current player mute or not.
     */
    void setMute(bool mute);
    /*!
     * Set current player volume.
     */
    void setVolume(int value);
    /*!
     * Get current player is playing or not.
     */
    bool isPlaying() const;
    /*!
     * Get current player state.
     */
    inline MusicObject::PlayState state() const { return m_playState; }
    /*!
     * Get current player category.
     */
    inline Category category() const { return m_category; }

Q_SIGNALS:
    /*!
     * Current position changed.
     */
    void positionChanged(qint64 position);
    /*!
     * Current duration changed.
     */
    void durationChanged(qint64 duration);
    /*!
     * Current volume changed.
     */
    void volumeChanged(int volume);
    /*!
     * Current media data path changed.
     */
    void mediaChanged(const QString &data);
    /*!
     * Current state changed.
     */
    void stateChanged(MusicObject::PlayState state);
    /*!
     * Current media is radio changed.
     */
    void radioChanged();
    /*!
     * Current media is radio changed.
     */
    void finished();

public Q_SLOTS:
    /*!
     * Player to play.
     */
    void play();
    /*!
     * Player to stop.
     */
    void stop();

private Q_SLOTS:
    /*!
     * Player data has recieved.
     */
    void dataRecieve();
    /*!
     * Player position data has recieved.
     */
    void positionRecieve();
    /*!
     * Player duration data has recieved.
     */
    void durationRecieve();
    /*!
     * Player radio data has recieved.
     */
    void radioStandardRecieve();
    /*!
     * Player music data has recieved.
     */
    void musicStandardRecieve();
    /*!
     * Player one second time out.
     */
    void timeout();

protected:
    /*!
     * Set player to video media data.
     */
    void setVideoMedia(const QString &data, int winId);
    /*!
     * Set player to music media data.
     */
    void setMusicMedia(const QString &data);
    /*!
     * Set player to radio media data.
     */
    void setRadioMedia(const QString &data);

    QProcess *m_process;
    MusicObject::PlayState m_playState;
    Category m_category;
    QTimer m_timer;

};

#endif // MUSICCOREMPLAYER_H
