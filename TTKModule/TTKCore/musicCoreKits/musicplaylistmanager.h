#ifndef MUSICPLAYLISTMANAGER_H
#define MUSICPLAYLISTMANAGER_H

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

/*! @brief The class of the playlist manager.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPlaylistManager
{
    TTK_DECLARE_MODULE(MusicPlaylistManager)
public:
    /*!
     * Write music playlist data to file.
     */
    void writeSongItem(const QString &path, const MusicSongItem &item);
    /*!
     * Read music playlist data from file.
     */
    void readSongItems(const QStringList &paths, MusicSongItemList &items);

private:
    /*!
     * Read ttk music playlist data from file.
     */
    bool readTKPLConfig(const QString &path, MusicSongItemList &items);
    /*!
     * Write ttk music playlist data to file.
     */
    bool writeTKPLConfig(const QString &path, const MusicSongItem &item);

    /*!
     * Read m3u music playlist data from file.
     */
    bool readM3UConfig(const QString &path, MusicSongItemList &items);
    /*!
     * Write m3u music playlist data to file.
     */
    bool writeM3UConfig(const QString &path, const MusicSongItem &item);

    /*!
     * Read pls music playlist data from file.
     */
    bool readPLSConfig(const QString &path, MusicSongItemList &items);
    /*!
     * Write pls music playlist data to file.
     */
    bool writePLSConfig(const QString &path, const MusicSongItem &item);

    /*!
     * Read wpl music playlist data from file.
     */
    bool readWPLConfig(const QString &path, MusicSongItemList &items);
    /*!
     * Write wpl music playlist data to file.
     */
    bool writeWPLConfig(const QString &path, const MusicSongItem &item);

    /*!
     * Read xspf music playlist data from file.
     */
    bool readXSPFConfig(const QString &path, MusicSongItemList &items);
    /*!
     * Write xspf music playlist data to file.
     */
    bool writeXSPFConfig(const QString &path, const MusicSongItem &item);

    /*!
     * Read jspf music playlist data from file.
     */
    bool readJSPFConfig(const QString &path, MusicSongItemList &items);
    /*!
     * Write jspf music playlist data to file.
     */
    bool writeJSPFConfig(const QString &path, const MusicSongItem &item);

    /*!
     * Read asx music playlist data from file.
     */
    bool readASXConfig(const QString &path, MusicSongItemList &items);
    /*!
     * Write asx music playlist data to file.
     */
    bool writeASXConfig(const QString &path, const MusicSongItem &item);

    /*!
     * Read foobar2k music playlist data from file.
     */
    bool readFPLConfig(const QString &path, MusicSongItemList &items);

    /*!
     * Read deadbeef music playlist data from file.
     */
    bool readDBPLConfig(const QString &path, MusicSongItemList &items);

    /*!
     * Read csv music playlist data from file.
     */
    bool readCSVConfig(const QString &path, MusicSongItemList &items);
    /*!
     * Write csv music playlist data to file.
     */
    bool writeCSVConfig(const QString &path, const MusicSongItem &item);

    /*!
     * Read txt music playlist data from file.
     */
    bool readTXTConfig(const QString &path, MusicSongItemList &items);
    /*!
     * Write txt music playlist data to file.
     */
    bool writeTXTConfig(const QString &path, const MusicSongItem &item);

};

#endif // MUSICPLAYLISTMANAGER_H
