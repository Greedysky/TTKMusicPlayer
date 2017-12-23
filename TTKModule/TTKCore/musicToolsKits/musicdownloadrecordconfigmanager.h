#ifndef MUSICDOWNLOADRECORDCONFIGMANAGER_H
#define MUSICDOWNLOADRECORDCONFIGMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

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

#include "musicabstractxml.h"

/*! @brief The class of the local download record item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_TOOL_EXPORT MusicDownloadRecord
{
    QString m_name;
    QString m_path;
    QString m_size;
}MusicDownloadRecord;
MUSIC_DECLARE_LISTS(MusicDownloadRecord)

/*! @brief The class of the download record manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicDownloadRecordConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    enum Type
    {
        Normal,         /*!< Local File Config*/
        Cloud           /*!< Cloud File Config*/
    };

    /*!
     * Object contsructor.
     */
    explicit MusicDownloadRecordConfigManager(Type type, QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set config type.
     */
    inline void setType(Type type) { m_type = type; }
    /*!
     * Get config type.
     */
    inline Type getType() const { return m_type; }
    /*!
     * Read history download datas from xml file by given name.
     */
    inline bool readDownloadXMLConfig() { return readConfig( mappingFilePathFromEnum() ); }
    /*!
     * Write history download datas into xml file.
     */
    void writeDownloadConfig(const MusicDownloadRecords &records);
    /*!
     * Read history download datas into xml file.
     */
    void readDownloadConfig(MusicDownloadRecords &records);

protected:
    /*!
     * Mapping file path from enum type.
     */
    QString mappingFilePathFromEnum() const;

    Type m_type;

};

#endif // MUSICDOWNLOADRECORDCONFIGMANAGER_H
