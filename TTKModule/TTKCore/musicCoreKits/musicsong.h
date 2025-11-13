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
#include "musicsonghelper.h"

/*! @brief The class of the music song info.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSong
{
public:
    enum class Sort
    {
        ByFileName,      /*!< Sort by file name */
        BySinger,        /*!< Sort by singer name */
        ByFileSize,      /*!< Sort by file size */
        ByAddTime,       /*!< Sort by add time */
        ByDuration,      /*!< Sort by duration */
        ByPlayCount      /*!< Sort by play count */
    };

    /*!
     * Object constructor.
     */
    MusicSong() noexcept;
    explicit MusicSong(const QString &path, bool track = false);
    MusicSong(const QString &path, const QString &duration, const QString &name = {}, bool track = false);

    /*!
     * Get media title name.
     */
    QString title() const noexcept;
    /*!
     * Get media artist name.
     */
    QString artist() const noexcept;

    /*!
     * Set media add time string.
     */
    inline void setAddTimeStr(const QString &t) noexcept { m_addTimeStr = t; }
    /*!
     * Get media add time string.
     */
    inline QString addTimeStr() const noexcept { return m_addTimeStr; }
    /*!
     * Set media size string.
     */
    inline void setSizeStr(const QString &s) noexcept { m_sizeStr = s; }
    /*!
     * Get media size string.
     */
    inline QString sizeStr() const noexcept { return m_sizeStr; }

    /*!
     * Set media name.
     */
    inline void setName(const QString &n) noexcept { m_name = n; }
    /*!
     * Get media name.
     */
    inline QString name() const noexcept { return m_name; }
    /*!
     * Set media path.
     */
    inline void setPath(const QString &p) noexcept { m_path = p; }
    /*!
     * Get media path.
     */
    inline QString path() const noexcept { return m_path; }
    /*!
     * Set media format.
     */
    inline void setFormat(const QString &t) noexcept { m_format = t; }
    /*!
     * Get media format.
     */
    inline QString format() const noexcept { return m_format; }
    /*!
     * Set media duration.
     */
    inline void setDuration(const QString &t) noexcept { m_duration = t; }
    /*!
     * Get media duration.
     */
    inline QString duration() const noexcept { return m_duration; }
    /*!
     * Get media size.
     */
    inline qint64 size() const noexcept { return m_size; }
    /*!
     * Set media play count.
     */
    inline void setPlayCount(const int c) noexcept { m_playCount = c; }
    /*!
     * Get media play count.
     */
    inline int playCount() const noexcept { return m_playCount; }
    /*!
     * Set media sort type.
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
     * Generate song playlist.
     */
    TTK_MODULE_EXPORT MusicSongList generateSongList(const QString &path);

}

#endif // MUSICSONG_H
