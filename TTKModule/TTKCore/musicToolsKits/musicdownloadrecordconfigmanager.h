#ifndef MUSICDOWNLOADRECORDCONFIGMANAGER_H
#define MUSICDOWNLOADRECORDCONFIGMANAGER_H

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

#include "ttkabstractxml.h"
#include "musicsong.h"
#include "musicnetworkdefines.h"

/*! @brief The namespace of the record type helper.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    /*!
     * Mapping file path from enum type.
     */
    TTK_MODULE_EXPORT QString toString(Record type) noexcept;

}


/*! @brief The class of the download record manager.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDownloadRecordConfigManager : public TTKAbstractXml, public TTKAbstractReadWriteInterface<MusicSongList>
{
public:
    /*!
     * Object constructor.
     */
    MusicDownloadRecordConfigManager();

    /*!
     * Read datas from buffer.
     */
    virtual bool readBuffer(MusicSongList &items) override final;
    /*!
     * Write datas into buffer.
     */
    virtual bool writeBuffer(const MusicSongList &items) override final;

};

#endif // MUSICDOWNLOADRECORDCONFIGMANAGER_H
