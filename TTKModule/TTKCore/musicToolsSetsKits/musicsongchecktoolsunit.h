#ifndef MUSICSONGCHECKTOOLSUNIT_H
#define MUSICSONGCHECKTOOLSUNIT_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

#include "musicsong.h"

/*! @brief The namespace of the mode type data.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    enum class Mode
    {
        Check,      /*!< Song Check Mode*/
        Apply       /*!< Song Apply Mode*/
    };
}

/*! @brief The class of the song check tools rename.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicSongCheckToolsRename
{
    QString m_locaName;
    QString m_recommendName;
    QString m_path;

    MusicSongCheckToolsRename(const QString &locaName, const QString &recommendName, const QString &path)
        : m_locaName(locaName),
          m_recommendName(recommendName),
          m_path(path)
    {

    }
};
TTK_DECLARE_LIST(MusicSongCheckToolsRename);


/*! @brief The class of the song check tools duplicate.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicSongCheckToolsDuplicate
{
    MusicSong m_song;
    QString m_bitrate;

    MusicSongCheckToolsDuplicate(const MusicSong &song, const QString &bitrate)
        : m_song(song),
          m_bitrate(bitrate)
    {

    }
};
TTK_DECLARE_LIST(MusicSongCheckToolsDuplicate);


/*! @brief The class of the song check tools quality.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicSongCheckToolsQuality
{
    MusicSong m_song;
    QString m_bitrate;

    MusicSongCheckToolsQuality(const MusicSong &song, const QString &bitrate)
        : m_song(song),
          m_bitrate(bitrate)
    {

    }
};
TTK_DECLARE_LIST(MusicSongCheckToolsQuality);

#endif // MUSICSONGCHECKTOOLSUNIT_H
