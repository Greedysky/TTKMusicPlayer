#ifndef MUSICPLSCONFIGMANAGER_H
#define MUSICPLSCONFIGMANAGER_H

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

#include "musicplaylistinterface.h"

/*! @brief The class of the pls playlist Config Manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicPLSConfigManager : public MusicPlaylistInterface
{
    TTK_DECLARE_MODULE(MusicPLSConfigManager)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicPLSConfigManager();

    /*!
     * Read datas from xml file by given name.
     */
    bool readConfig(const QString &name);

    /*!
     * Read datas from config file.
     */
    virtual void readPlaylistData(MusicSongItems &items) override;
    /*!
     * Write datas into config file.
     */
    virtual void writePlaylistData(const MusicSongItems &items, const QString &path) override;

private:
    QFile m_file;

};

#endif // MUSICPLSCONFIGMANAGER_H
