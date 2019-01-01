#ifndef MUSICCOREMPLAYER_H
#define MUSICCOREMPLAYER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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
 ================================================= */

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
    TTK_DECLARE_MODULE(MusicCoreMPlayer)
public:
    enum Category
    {
        NullCategory,   /*!< no category*/
        MusicCategory,  /*!< music category*/
        VideoCategory   /*!< video category*/
    };

    /*!
     * Object contsructor.
     */
    explicit MusicCoreMPlayer(QObject *parent = nullptr);

    ~MusicCoreMPlayer();

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
     * Current media is finished.
     */
    void finished(int code);

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
     * Player music data has recieved.
     */
    void musicStandardRecieve();
    /*!
     * Player one second time out.
     */
    void timeout();
    /*!
     * Check thread time out.
     */
    void checkTimerout();

protected:
    /*!
     * Set player to video media data.
     */
    void setVideoMedia(const QString &data, int winId);
    /*!
     * Set player to music media data.
     */
    void setMusicMedia(const QString &data);

    QProcess *m_process;
    MusicObject::PlayState m_playState;
    Category m_category;
    QTimer m_timer, m_checkTimer;

};

#endif // MUSICCOREMPLAYER_H
