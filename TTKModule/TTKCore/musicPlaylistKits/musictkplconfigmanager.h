#ifndef MUSICTKPLCONFIGMANAGER_H
#define MUSICTKPLCONFIGMANAGER_H

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

#include "ttkabstractxml.h"
#include "musicplaylistinterface.h"

/*! @brief The class of the tkpl config manager.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicTKPLConfigManager : public TTKAbstractXml, public MusicPlaylistInterface
{
    TTK_DECLARE_MODULE(MusicTKPLConfigManager)
public:
    /*!
     * Object constructor.
     */
    MusicTKPLConfigManager();

    /*!
     * Read datas from buffer.
     */
    virtual bool readBuffer(MusicSongItemList &items) override final;
    /*!
     * Write datas into buffer.
     */
    virtual bool writeBuffer(const MusicSongItemList &items) override final;

private:
    /*!
     * Read media File Path.
     */
    MusicSongList readMusicFilePath(const QDomNode &node) const;

};

#endif // MUSICTKPLCONFIGMANAGER_H
