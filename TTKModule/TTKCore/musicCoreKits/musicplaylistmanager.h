#ifndef MUSICPLAYLISTMANAGER_H
#define MUSICPLAYLISTMANAGER_H

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

#include "musicsong.h"

/*! @brief The class of the playlist manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicPlaylistManager
{
    TTK_DECLARE_MODULE(MusicPlaylistManager)
public:
    /*!
     * Write music playlist data to file.
     */
    void setMusicSongItem(const QString &path, const MusicSongItem &item);
    /*!
     * Read music playlist data from file.
     */
    void getMusicSongItems(const QStringList &paths, MusicSongItems &items);

protected:
    /*!
     * Read ttk music playlist data from file.
     */
    void readLisList(const QString &path, MusicSongItems &items);
    /*!
     * Write ttk music playlist data to file.
     */
    void writeLisList(const QString &path, const MusicSongItem &item);

    /*!
     * Read m3u music playlist data from file.
     */
    void readM3UList(const QString &path, MusicSongItems &items);
    /*!
     * Write m3u music playlist data to file.
     */
    void writeM3UList(const QString &path, const MusicSongItem &item);

    /*!
     * Read pls music playlist data from file.
     */
    void readPLSList(const QString &path, MusicSongItems &items);
    /*!
     * Write pls music playlist data to file.
     */
    void writePLSList(const QString &path, const MusicSongItem &item);

    /*!
     * Read wpl music playlist data from file.
     */
    void readWPLList(const QString &path, MusicSongItems &items);
    /*!
     * Write wpl music playlist data to file.
     */
    void writeWPLList(const QString &path, const MusicSongItem &item);

    /*!
     * Read xspf music playlist data from file.
     */
    void readXSPFList(const QString &path, MusicSongItems &items);
    /*!
     * Write xspf music playlist data to file.
     */
    void writeXSPFList(const QString &path, const MusicSongItem &item);

    /*!
     * Read asx music playlist data from file.
     */
    void readASXList(const QString &path, MusicSongItems &items);
    /*!
     * Write asx music playlist data to file.
     */
    void writeASXList(const QString &path, const MusicSongItem &item);

    /*!
     * Read kuwo music playlist data from file.
     */
    void readKWLList(const QString &path, MusicSongItems &items);

    /*!
     * Read kugou music playlist data from file.
     */
    void readKGLList(const QString &path, MusicSongItems &items);

    /*!
     * Read foobar2k music playlist data from file.
     */
    void readFPLList(const QString &path, MusicSongItems &items);

};

#endif // MUSICPLAYLISTMANAGER_H
