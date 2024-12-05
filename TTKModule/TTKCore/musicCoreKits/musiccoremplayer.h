#ifndef MUSICCOREMPLAYER_H
#define MUSICCOREMPLAYER_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

class QProcess;

/*! @brief The class of the mplayer core.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicCoreMPlayer : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicCoreMPlayer)
public:
    enum class Module
    {
        Null,   /*!< no module*/
        Radio,  /*!< radio module*/
        Music,  /*!< music module*/
        Video,  /*!< video module*/
        Movie,  /*!< movie module*/
    };

    /*!
     * Object constructor.
     */
    explicit MusicCoreMPlayer(QObject *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicCoreMPlayer();

    /*!
     * Set media by type and url path.
     */
    void setMedia(Module type, const QString &url, int winId = -1);

    /*!
     * Set current play pos.
     */
    void setPosition(qint64 pos);

    /*!
     * Set current player mute or not.
     */
    void setMuted(bool mute);
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
    inline TTK::PlayState state() const { return m_playState; }
    /*!
     * Get current player category.
     */
    inline Module category() const { return m_category; }

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
     * Current media data path changed.
     */
    void mediaChanged(const QString &url);
    /*!
     * Current media is finished.
     */
    void finished(int code);

public Q_SLOTS:
    /*!
     * Player to play or pause.
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
    void dataRecieved();
    /*!
     * Player position data has recieved.
     */
    void positionRecieved();
    /*!
     * Player duration data has recieved.
     */
    void durationRecieved();
    /*!
     * Player music data has recieved.
     */
    void standardRecieved();
    /*!
     * Player one second time out.
     */
    void timeout();
    /*!
     * Check thread time out.
     */
    void checkTimerout();

private:
    /*!
     * Close output media module.
     */
    void closeModule();
    /*!
     * Set player to radio media data.
     */
    void setRadioMedia(const QString &url);
    /*!
     * Set player to music media data.
     */
    void setMusicMedia(const QString &url);
    /*!
     * Set player to video media data.
     */
    void setVideoMedia(const QString &url, int winId);
    /*!
     * Set player to movie media data.
     */
    void setMovieMedia(const QString &url, int winId);

    QProcess *m_process;
    TTK::PlayState m_playState;
    Module m_category;
    QTimer m_timer, m_checkTimer;

};

#endif // MUSICCOREMPLAYER_H
