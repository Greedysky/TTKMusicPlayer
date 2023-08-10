#ifndef MUSICCONSOLEMODULE_H
#define MUSICCONSOLEMODULE_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

#include "musicobject.h"
#include "ttkcommandline.h"

class MusicPlayer;
class MusicPlaylist;

/*! @brief The class of the music console module.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicConsoleModule : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicConsoleModule(QObject *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicConsoleModule();

    /*!
     * Init all parameter.
     */
    bool initialize(const QCoreApplication &app);

private Q_SLOTS:
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
    void switchPlayState();
    /*!
     * Set current player to play previous.
     */
    void playPrevious();
    /*!
     * Set current player to play next.
     */
    void playNext();

    /*!
     * Reset player volume.
     */
    void resetVolume();
    /*!
     * Set current player volume down.
     */
    void volumeDown();
    /*!
     * Set current player volume up.
     */
    void volumeUp();

    /*!
     * Set current play mdoe to order.
     */
    void playOrder();
    /*!
     * Set current play mdoe to random.
     */
    void playRandom();
    /*!
     * Set current play mdoe to list loop.
     */
    void playlistLoop();
    /*!
     * Set current play mdoe to single loop.
     */
    void playOneLoop();
    /*!
     * Set current play mdoe to just once.
     */
    void playOnce();

    /*!
     * Set current music enhanced off, no any effect.
     */
    void setEnhancedOff();
    /*!
     * Set current music 3d music effect.
     */
    void setEnhanced3D();
    /*!
     * Set current music nicam music effect.
     */
    void setEnhancedNICAM();
    /*!
     * Set current music subwoofer music effect.
     */
    void setEnhancedSubwoofer();
    /*!
     * Set current music vocal music effect.
     */
    void setEnhancedVocal();

private:
    /*!
     * Print debug info.
     */
    void print(qint64 position, qint64 duration) const;

    MusicPlayer* m_player;
    MusicPlaylist* m_playlist;
    int m_volume;
    QString m_playbackMode, m_enhanced;

};

#endif // MUSICCONSOLEMODULE_H
