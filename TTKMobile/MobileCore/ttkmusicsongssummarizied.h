#ifndef TTKMUSICSONGSSUMMARIZIED_H
#define TTKMUSICSONGSSUMMARIZIED_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2017 Greedysky Studio

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

#include "musicsong.h"
#include "musicmobileglobaldefine.h"

/*! @brief The class of the music songs summarizied.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_MOBILE_EXPORT TTKMusicSongsSummarizied : public QObject
{
    Q_OBJECT
public:
    explicit TTKMusicSongsSummarizied(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    void addMusicLists(const MusicSongItems &names);
    /*!
     * Add music datas into container.
     */
    void appendMusicLists(const MusicSongItems &names);
    /*!
     * Append music datas into container.
     */
    MusicSongItem getMusicList(int index);
    /*!
     * Get music data from container.
     */

    inline const MusicSongItems& getMusicLists() const  { return m_songItems;}
    /*!
     * Get music datas from container.
     */
    void importOtherMusicSongs(int index, const QStringList &filelist);
    /*!
     * Input orther imported music datas into container.
     */
    void importNetworkMusicSongs(const QString &key, const QString &path);
    /*!
     * Input network imported music datas into container.
     */
    void setRecentMusicSongs(int index);
    /*!
     * Input recent imported music datas into container.
     */

    void removeMusicSongs(int tool, int index);
    /*!
     * Remove music data from container.
     */
    void removeMusicSongs(int tool, const QString &path);
    /*!
     * Remove music data from container.
     */

    QStringList getMusicSongsFileName(int index) const;
    /*!
     * Get music songs file name by index.
     */
    QStringList getMusicSongsFilePath(int index) const;
    /*!
     * Get music songs file path by index.
     */

    inline void setToolBoxIndex(int index) { m_currentToolIndex = index; }
    /*!
     * Set current selected tool box index.
     */
    inline int getToolBoxIndex() const { return m_currentToolIndex; }
    /*!
     * Get current selected tool box index.
     */
    inline void setCurrentIndex(int index) { m_currentPlayIndex = index; }
    /*!
     * Set current played tool box index.
     */
    inline int getCurrentIndex() const { return m_currentPlayIndex; }
    /*!
     * Get current played tool box index.
     */

protected:
    int m_currentPlayIndex, m_currentToolIndex;
    MusicSongItems m_songItems;

};

#endif // TTKMUSICSONGSSUMMARIZIED_H
