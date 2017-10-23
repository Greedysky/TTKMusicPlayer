#ifndef MUSICSONG_H
#define MUSICSONG_H

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

#include "musicobject.h"
#include "musicglobaldefine.h"

/*! @brief The class of the music song info.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicSong
{
public:
    enum Sort
    {
        SortByFileName = 0,      /*!< Sort by file name*/
        SortBySinger,            /*!< Sort by singer name*/
        SortByFileSize,          /*!< Sort by file size*/
        SortByAddTime,           /*!< Sort by add time*/
        SortByPlayTime,          /*!< Sort by play time*/
        SortByPlayCount          /*!< Sort by play count*/
    };

    /*!
     * Object contsructor.
     */
    MusicSong();

    /*!
     * Object contsructor by music path and name.
     */
    explicit MusicSong(const QString &musicPath,
                       const QString &musicName = QString());
    /*!
     * Object contsructor by music path and name and playcout.
     */
    MusicSong(const QString &musicPath, int playCount,
              const QString &musicName = QString());
    /*!
     * Object contsructor by music path and name and playcout and format.
     */
    MusicSong(const QString &musicPath, const QString &type,
              int playCount, const QString &musicName = QString());
    /*!
     * Object contsructor by music path and name and playcout and format and time.
     */
    MusicSong(const QString &musicPath, const QString &type,
              const QString &time, int playCount,
              const QString &musicName = QString());
    /*!
     * Object contsructor by music path and name and playcout and time.
     */
    MusicSong(const QString &musicPath, int playCount, const QString &time,
              const QString &musicName);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Get music artist name.
     */
    QString getMusicArtistFront() const;
    /*!
     * Get music name.
     */
    QString getMusicArtistBack() const;
    /*!
     * Set music name.
     */
    inline void setMusicName(const QString &n) { m_musicName = n;}
    /*!
     * Get music name.
     */
    inline QString getMusicName() const { return m_musicName;}
    /*!
     * Set music path.
     */
    inline void setMusicPath(const QString &p) { m_musicPath = p;}
    /*!
     * Get music path.
     */
    inline QString getMusicPath() const { return m_musicPath;}
    /*!
     * Set music format.
     */
    inline void setMusicType(const QString &t) { m_musicType = t;}
    /*!
     * Get music format.
     */
    inline QString getMusicType() const { return m_musicType;}
    /*!
     * Set music time.
     */
    inline void setMusicTime(const QString &t) { m_musicTime = t;}
    /*!
     * Get music time.
     */
    inline QString getMusicTime() const { return m_musicTime;}
    /*!
     * Set music size.
     */
    inline void setMusicSize(const qint64 s) { m_musicSize = s;}
    /*!
     * Get music size.
     */
    inline qint64 getMusicSize() const { return m_musicSize;}
    /*!
     * Set music play count.
     */
    inline void setMusicPlayCount(const int c) { m_musicPlayCount = c;}
    /*!
     * Get music play count.
     */
    inline int getMusicPlayCount() const { return m_musicPlayCount;}
    /*!
     * Set music sort type.
     */
    inline void setMusicSort(const Sort s) { m_sortType = s;}
    /*!
     * Get music sort type.
     */
    inline Sort getMusicSort() const { return m_sortType;}
    /*!
     * Operator == function.
     */
    bool operator== (const MusicSong &other) const;
    /*!
     * Operator < function.
     */
    bool operator< (const MusicSong &other) const;

protected:
    Sort m_sortType;
    qint64  m_musicSize, m_musicAddTime;
    int m_musicPlayCount;
    QString m_musicName, m_musicPath, m_musicType, m_musicTime;

};
MUSIC_DECLARE_LISTS(MusicSong)

////////////////////////////////////////////////////////
class MusicSongsListTableWidget;

/*! @brief The class of the music song sort tag.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_CORE_EXPORT MusicSort
{
    int m_index;
    Qt::SortOrder m_sortType;

    MusicSort()
    {
        m_index = -1;
        m_sortType = Qt::AscendingOrder;
    }
}MusicSort;

/*! @brief The class of the music song item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_CORE_EXPORT MusicSongItem
{
    int m_itemIndex;
    QString m_itemName;
    MusicSort m_sort;
    MusicSongs m_songs;
    MusicSongsListTableWidget* m_itemObject;

    MusicSongItem()
    {
        m_itemIndex = -1;
        m_itemObject = nullptr;
    }

    inline bool operator<(const MusicSongItem &other) const
    {
        return m_itemIndex < other.m_itemIndex;
    }
}MusicSongItem;
MUSIC_DECLARE_LISTS(MusicSongItem)

#endif // MUSICSONG_H
