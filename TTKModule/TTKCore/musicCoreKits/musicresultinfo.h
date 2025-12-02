#ifndef MUSICRESULTINFO_H
#define MUSICRESULTINFO_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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

#include "musicglobaldefine.h"

/*! @brief The class of the search result info item.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicResultInfoItem
{
    QString m_songName;
    QString m_artistName;
    QString m_albumName;
    QString m_duration;
    QString m_type;

    MusicResultInfoItem() noexcept
        : m_songName(TTK_DEFAULT_STR),
          m_artistName(TTK_DEFAULT_STR),
          m_albumName(TTK_DEFAULT_STR),
          m_duration(TTK_DEFAULT_STR),
          m_type(TTK_DEFAULT_STR)
    {

    }

    MusicResultInfoItem(const TTK::MusicSongInformation &info) noexcept
        : m_songName(info.m_songName),
          m_artistName(info.m_artistName),
          m_albumName(info.m_albumName),
          m_duration(info.m_duration)
    {

    }

    MusicResultInfoItem(const TTK::MusicSongInformation &info, const QString &type) noexcept
        : MusicResultInfoItem(info)
    {
        m_type = type;
    }
};

/*! @brief The class of the search result data item.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicResultDataItem
{
    QString m_id;
    QString m_name;
    QString m_title;
    QString m_nickName;
    QString m_coverUrl;
    QString m_count;
    QString m_description;
    QString m_time;
    QString m_category;

    MusicResultDataItem() noexcept
        : m_id(TTK_DEFAULT_STR),
          m_name(TTK_DEFAULT_STR),
          m_title(TTK_DEFAULT_STR),
          m_nickName(TTK_DEFAULT_STR),
          m_coverUrl(TTK_DEFAULT_STR),
          m_count(TTK_DEFAULT_STR),
          m_description(TTK_DEFAULT_STR),
          m_time(TTK_DEFAULT_STR),
          m_category(TTK_DEFAULT_STR)
    {

    }

    inline bool isEmpty() const noexcept
    {
        return m_name == TTK_DEFAULT_STR || m_nickName == TTK_DEFAULT_STR || m_coverUrl == TTK_DEFAULT_STR || m_description == TTK_DEFAULT_STR || m_category == TTK_DEFAULT_STR;
    }
};
TTK_DECLARE_LIST(MusicResultDataItem);

#endif // MUSICRESULTINFO_H
