#ifndef MUSICCONSOLEOBJECT_H
#define MUSICCONSOLEOBJECT_H

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

#include <QDebug>
#include <QCoreApplication>
#include "musicobject.h"
#include "musicrunglobaldefine.h"

class MusicPlayer;
class MusicPlaylist;

/*! @brief The class of the music console object.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_RUN_EXPORT MusicConsoleObject : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicConsoleObject(QObject *parent = nullptr);

    ~MusicConsoleObject();

    /*!
     * Init all parameter.
     */
    bool init(const QCoreApplication &app);

private Q_SLOTS:
    /*!
     * Current duration changed.
     */
    void durationChanged(qint64 duration);
    /*!
     * Current position changed.
     */
    void positionChanged(qint64 position);
    /*!
     * Current playlist index changed.
     */
    void currentIndexChanged(int index);

    /*!
     * Set current player to play or not.
     */
    void musicStatePlay();
    /*!
     * Set current player to play previous.
     */
    void musicPlayPrevious();
    /*!
     * Set current player to play next.
     */
    void musicPlayNext();

    /*!
     * Reset player volume.
     */
    void resetVolume();
    /*!
     * Set current player volume down.
     */
    void musicActionVolumeSub();
    /*!
     * Set current player volume up.
     */
    void musicActionVolumePlus();

    /*!
     * Set current play mdoe to order.
     */
    void musicPlayOrder();
    /*!
     * Set current play mdoe to random.
     */
    void musicPlayRandom();
    /*!
     * Set current play mdoe to list loop.
     */
    void musicPlaylistLoop();
    /*!
     * Set current play mdoe to single loop.
     */
    void musicPlayOneLoop();
    /*!
     * Set current play mdoe to just once.
     */
    void musicPlayItemOnce();

    /*!
     * Set current music enhanced off, no any effect.
     */
    void musicEnhancedOff();
    /*!
     * Set current music 3d music effect.
     */
    void musicEnhanced3D();
    /*!
     * Set current music nicam music effect.
     */
    void musicEnhancedNICAM();
    /*!
     * Set current music subwoofer music effect.
     */
    void musicEnhancedSubwoofer();
    /*!
     * Set current music vocal music effect.
     */
    void musicEnhancedVocal();

protected:
    /*!
     * Print debug info.
     */
    void print(qint64 position, qint64 duration);

    MusicPlayer* m_musicPlayer;
    MusicPlaylist* m_musicPlaylist;
    int m_volume;
    QString m_playbackMode, m_enhanced;

};

#endif // MUSICCONSOLEOBJECT_H
