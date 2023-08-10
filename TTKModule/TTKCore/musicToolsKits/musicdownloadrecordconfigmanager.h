#ifndef MUSICDOWNLOADRECORDCONFIGMANAGER_H
#define MUSICDOWNLOADRECORDCONFIGMANAGER_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

/*! @brief The class of the download record manager.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDownloadRecordConfigManager : public TTKXmlDocument
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownloadRecordConfigManager)
public:
    /*!
     * Object constructor.
     */
    explicit MusicDownloadRecordConfigManager(TTK::Record type, QObject *parent = nullptr);

    /*!
     * Set config type.
     */
    inline void setType(TTK::Record type) { m_type = type; }
    /*!
     * Get config type.
     */
    inline TTK::Record type() const { return m_type; }

    /*!
     * Read datas from xml file.
     */
    inline bool fromFile() { return TTKXmlDocument::fromFile(mappingFilePathFromEnum()); }

    /*!
     * Read datas from config file.
     */
    void readBuffer(MusicSongList &items);
    /*!
     * Write datas into config file.
     */
    void writeBuffer(const MusicSongList &items);

private:
    /*!
     * Mapping file path from enum type.
     */
    QString mappingFilePathFromEnum() const;

    TTK::Record m_type;

};

#endif // MUSICDOWNLOADRECORDCONFIGMANAGER_H
