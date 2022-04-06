#ifndef MUSICSONG_H
#define MUSICSONG_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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

#include "musictime.h"
#include "musicglobaldefine.h"

/*! @brief The class of the music song info.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSong
{
    TTK_DECLARE_MODULE(MusicSong)
public:
    enum Sort
    {
        SortByFileName,      /*!< Sort by file name*/
        SortBySinger,        /*!< Sort by singer name*/
        SortByFileSize,      /*!< Sort by file size*/
        SortByAddTime,       /*!< Sort by add time*/
        SortByPlayTime,      /*!< Sort by play time*/
        SortByPlayCount      /*!< Sort by play count*/
    };

    /*!
     * Object contsructor.
     */
    MusicSong();
    /*!
     * Object contsructor by music path and name.
     */
    explicit MusicSong(const QString &musicPath, bool track = false);
    /*!
     * Object contsructor by music path and name and playcout and time.
     */
    MusicSong(const QString &musicPath, const QString &playTime, const QString &musicName = QString(), bool track = false);

    /*!
     * Get music artist name.
     */
    QString musicArtistFront() const;
    /*!
     * Get music song name.
     */
    QString musicArtistBack() const;

    /*!
     * Set music add time string.
     */
    inline void setMusicAddTimeStr(const QString &t) { m_musicAddTimeStr = t; }
    /*!
     * Get music add time string.
     */
    inline QString musicAddTimeStr() const { return m_musicAddTimeStr; }
    /*!
     * Set music size string.
     */
    inline void setMusicSizeStr(const QString &s) { m_musicSizeStr = s; }
    /*!
     * Get music size string.
     */
    inline QString musicSizeStr() const { return m_musicSizeStr; }

    /*!
     * Set music name.
     */
    inline void setMusicName(const QString &n) { m_musicName = n; }
    /*!
     * Get music name.
     */
    inline QString musicName() const { return m_musicName; }
    /*!
     * Set music path.
     */
    inline void setMusicPath(const QString &p) { m_musicPath = p; }
    /*!
     * Get music path.
     */
    inline QString musicPath() const { return m_musicPath; }
    /*!
     * Set music format.
     */
    inline void setMusicType(const QString &t) { m_musicType = t; }
    /*!
     * Get music format.
     */
    inline QString musicType() const { return m_musicType; }
    /*!
     * Set music time.
     */
    inline void setMusicPlayTime(const QString &t) { m_musicPlayTime = t; }
    /*!
     * Get music time.
     */
    inline QString musicPlayTime() const { return m_musicPlayTime; }
    /*!
     * Get music size.
     */
    inline qint64 musicSize() const { return m_musicSize; }
    /*!
     * Set music play count.
     */
    inline void setMusicPlayCount(const int c) { m_musicPlayCount = c; }
    /*!
     * Get music play count.
     */
    inline int musicPlayCount() const { return m_musicPlayCount; }
    /*!
     * Set music sort type.
     */
    inline void setMusicSort(const Sort s) { m_sortType = s; }

    /*!
     * Operator == function.
     */
    bool operator== (const MusicSong &other) const;
    /*!
     * Operator < function.
     */
    bool operator< (const MusicSong &other) const;
    /*!
     * Operator > function.
     */
    bool operator> (const MusicSong &other) const;

protected:
    Sort m_sortType;
    qint64 m_musicSize, m_musicAddTime;
    QString m_musicSizeStr, m_musicAddTimeStr;
    int m_musicPlayCount;
    QString m_musicName, m_musicPath, m_musicType, m_musicPlayTime;

};
TTK_DECLARE_LIST(MusicSong);


/*! @brief The class of the music song sort tag.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicSongSort
{
    int m_type;
    Qt::SortOrder m_order;

    MusicSongSort()
    {
        m_type = -1;
        m_order = Qt::AscendingOrder;
    }
};


class MusicAbstractSongsListTableWidget;

/*! @brief The class of the music song item.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicSongItem
{
    int m_itemIndex;
    QString m_itemName;
    MusicSongSort m_sort;
    MusicSongList m_songs;
    MusicAbstractSongsListTableWidget *m_itemObject;

    MusicSongItem()
    {
        m_itemIndex = -1;
        m_itemObject = nullptr;
    }

    inline bool operator<(const MusicSongItem &other) const
    {
        return m_itemIndex < other.m_itemIndex;
    }
};
TTK_DECLARE_LIST(MusicSongItem);


namespace MusicObject
{
    /*!
     * Remove track info in path.
     */
    TTK_MODULE_EXPORT QString trackRelatedPath(const QString &path);
    /*!
     * Generate music song playlist.
     */
    TTK_MODULE_EXPORT MusicSongList generateMusicSongList(const QString &path);
    /*!
     * Generate music song play time.
    */
    TTK_MODULE_EXPORT QString generateMusicPlayTime(const QString &path);
    /*!
     * Check current song playlist row is valid or not.
     */
    TTK_MODULE_EXPORT bool playlistRowValid(int index);

}

#endif // MUSICSONG_H
