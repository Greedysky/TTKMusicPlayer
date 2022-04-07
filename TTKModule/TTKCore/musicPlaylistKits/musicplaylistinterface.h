#ifndef MUSICPLAYLISTINTERFACE_H
#define MUSICPLAYLISTINTERFACE_H

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

#include "musicsong.h"

/*! @brief The class of the playlist reader.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPlaylistReader
{
public:
    explicit MusicPlaylistReader()
    {
    }

    virtual ~MusicPlaylistReader()
    {
    }

    /*!
     * Read datas from file by given name.
     */
    inline bool fromFile(const QString &name)
    {
        m_file.setFileName(name);
        return m_file.open(QFile::ReadOnly);
    }

protected:
    QFile m_file;

};

/*! @brief The class of the playlist interface.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPlaylistInterface
{
public:
    explicit MusicPlaylistInterface()
    {
    }

    virtual ~MusicPlaylistInterface()
    {
    }

    /*!
     * Read datas from config file.
     */
    virtual bool readBuffer(MusicSongItemList &items) = 0;
    /*!
     * Write datas into config file.
     */
    virtual bool writeBuffer(const MusicSongItemList &items, const QString &path) = 0;

};

#endif // MUSICPLAYLISTINTERFACE_H
