#ifndef MUSICPLAYLISTMANAGER_H
#define MUSICPLAYLISTMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2021 Greedysky Studio

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
    bool readTKPLList(const QString &path, MusicSongItems &items);
    /*!
     * Write ttk music playlist data to file.
     */
    bool writeTKPLList(const QString &path, const MusicSongItem &item);

    /*!
     * Read m3u music playlist data from file.
     */
    bool readM3UList(const QString &path, MusicSongItems &items);
    /*!
     * Write m3u music playlist data to file.
     */
    bool writeM3UList(const QString &path, const MusicSongItem &item);

    /*!
     * Read pls music playlist data from file.
     */
    bool readPLSList(const QString &path, MusicSongItems &items);
    /*!
     * Write pls music playlist data to file.
     */
    bool writePLSList(const QString &path, const MusicSongItem &item);

    /*!
     * Read wpl music playlist data from file.
     */
    bool readWPLList(const QString &path, MusicSongItems &items);
    /*!
     * Write wpl music playlist data to file.
     */
    bool writeWPLList(const QString &path, const MusicSongItem &item);

    /*!
     * Read xspf music playlist data from file.
     */
    bool readXSPFList(const QString &path, MusicSongItems &items);
    /*!
     * Write xspf music playlist data to file.
     */
    bool writeXSPFList(const QString &path, const MusicSongItem &item);

    /*!
     * Read asx music playlist data from file.
     */
    bool readASXList(const QString &path, MusicSongItems &items);
    /*!
     * Write asx music playlist data to file.
     */
    bool writeASXList(const QString &path, const MusicSongItem &item);

    /*!
     * Read kuwo music playlist data from file.
     */
    bool readKWLList(const QString &path, MusicSongItems &items);

    /*!
     * Read kugou music playlist data from file.
     */
    bool readKGLList(const QString &path, MusicSongItems &items);

    /*!
     * Read foobar2k music playlist data from file.
     */
    bool readFPLList(const QString &path, MusicSongItems &items);

    /*!
     * Read csv music playlist data from file.
     */
    bool readCSVList(const QString &path, MusicSongItems &items);
    /*!
     * Write csv music playlist data to file.
     */
    bool writeCSVList(const QString &path, const MusicSongItem &item);

    /*!
     * Read txt music playlist data from file.
     */
    bool readTXTList(const QString &path, MusicSongItems &items);
    /*!
     * Write txt music playlist data to file.
     */
    bool writeTXTList(const QString &path, const MusicSongItem &item);

    /*!
     * Write nfn music playlist data to file.
     */
    bool writeNFNList(const QString &path, const MusicSongItem &item);

};

#endif // MUSICPLAYLISTMANAGER_H
