#ifndef MUSICSONG_H
#define MUSICSONG_H

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

#include "ttktime.h"
#include "musicstringutils.h"

/*! @brief The class of the music song info.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSong
{
    TTK_DECLARE_MODULE(MusicSong)
public:
    enum class Sort
    {
        ByFileName,      /*!< Sort by file name*/
        BySinger,        /*!< Sort by singer name*/
        ByFileSize,      /*!< Sort by file size*/
        ByAddTime,       /*!< Sort by add time*/
        ByDuration,      /*!< Sort by duration*/
        ByPlayCount      /*!< Sort by play count*/
    };

    /*!
     * Object constructor.
     */
    MusicSong() noexcept;
    explicit MusicSong(const QString &path, bool track = false);
    MusicSong(const QString &path, const QString &duration, const QString &name = {}, bool track = false);

    /*!
     * Get music title name.
     */
    QString title() const noexcept;
    /*!
     * Get music artist name.
     */
    QString artist() const noexcept;

    /*!
     * Set music add time string.
     */
    inline void setAddTimeStr(const QString &t) noexcept { m_addTimeStr = t; }
    /*!
     * Get music add time string.
     */
    inline QString addTimeStr() const noexcept { return m_addTimeStr; }
    /*!
     * Set music size string.
     */
    inline void setSizeStr(const QString &s) noexcept { m_sizeStr = s; }
    /*!
     * Get music size string.
     */
    inline QString sizeStr() const noexcept { return m_sizeStr; }

    /*!
     * Set music name.
     */
    inline void setName(const QString &n) noexcept { m_name = n; }
    /*!
     * Get music name.
     */
    inline QString name() const noexcept { return m_name; }
    /*!
     * Set music path.
     */
    inline void setPath(const QString &p) noexcept { m_path = p; }
    /*!
     * Get music path.
     */
    inline QString path() const noexcept { return m_path; }
    /*!
     * Set music format.
     */
    inline void setFormat(const QString &t) noexcept { m_format = t; }
    /*!
     * Get music format.
     */
    inline QString format() const noexcept { return m_format; }
    /*!
     * Set music duration.
     */
    inline void setDuration(const QString &t) noexcept { m_duration = t; }
    /*!
     * Get music duration.
     */
    inline QString duration() const noexcept { return m_duration; }
    /*!
     * Get music size.
     */
    inline qint64 size() const noexcept { return m_size; }
    /*!
     * Set music play count.
     */
    inline void setPlayCount(const int c) noexcept { m_playCount = c; }
    /*!
     * Get music play count.
     */
    inline int playCount() const noexcept { return m_playCount; }
    /*!
     * Set music sort type.
     */
    inline void setSort(const Sort s) noexcept { m_sort = s; }

    bool operator== (const MusicSong &other) const noexcept;
    bool operator< (const MusicSong &other) const noexcept;
    bool operator> (const MusicSong &other) const noexcept;

private:
    Sort m_sort;
    qint64 m_size, m_addTime;
    QString m_sizeStr, m_addTimeStr;
    int m_playCount;
    QString m_name, m_path, m_format, m_duration;

};
TTK_DECLARE_LIST(MusicSong);


/*! @brief The class of the music song sort tag.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicSongSort
{
    int m_type;
    Qt::SortOrder m_order;

    MusicSongSort() noexcept
        : m_type(-1),
          m_order(Qt::AscendingOrder)
    {

    }
};


class MusicAbstractSongsListTableWidget;

/*! @brief The class of the music song item.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicSongItem
{
    int m_id;
    int m_itemIndex;
    QString m_itemName;
    MusicSongSort m_sort;
    MusicSongList m_songs;
    MusicAbstractSongsListTableWidget *m_itemWidget;

    MusicSongItem() noexcept
        : m_id(-1),
          m_itemIndex(-1),
          m_itemWidget(nullptr)
    {

    }

    inline bool operator<(const MusicSongItem &other) const noexcept
    {
        return m_itemIndex < other.m_itemIndex;
    }
};
TTK_DECLARE_LIST(MusicSongItem);


/*! @brief The namespace of the playlist helper.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    /*!
     * Check current song playlist row is valid or not.
     */
    TTK_MODULE_EXPORT bool playlistRowValid(int index) noexcept;
    /*!
     * Remove track info in path.
     */
    TTK_MODULE_EXPORT QString trackRelatedPath(const QString &path);

    /*!
     * Generate song name.
     */
    TTK_MODULE_EXPORT QString generateSongName(const QString &title, const QString &artist) noexcept;
    /*!
     * Get song title name.
     */
    TTK_MODULE_EXPORT QString generateSongTitle(const QString &name, const QString &key = TTK_DEFAULT_STR);
    /*!
     * Get song artist name.
     */
    TTK_MODULE_EXPORT QString generateSongArtist(const QString &name, const QString &key = TTK_DEFAULT_STR);
    /*!
     * Generate song playlist.
     */
    TTK_MODULE_EXPORT MusicSongList generateSongList(const QString &path);

    /*!
     * Generate network song play time.
    */
    TTK_MODULE_EXPORT QString generateNetworkSongTime(const QString &path);
    /*!
     * Generate network song path.
    */
    TTK_MODULE_EXPORT QString generateNetworkSongPath(const QString &path);

}

#endif // MUSICSONG_H
