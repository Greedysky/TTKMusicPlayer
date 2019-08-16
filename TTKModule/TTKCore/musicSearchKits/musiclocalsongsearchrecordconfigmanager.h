#ifndef MUSICLOCALSONGSEARCHRECORDCONFIGMANAGER_H
#define MUSICLOCALSONGSEARCHRECORDCONFIGMANAGER_H

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

#include "musicabstractxml.h"

/*! @brief The class of the local searched record item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_SEARCH_EXPORT MusicSearchRecord
{
    QString m_name;
    QString m_time;
}MusicSearchRecord;
TTK_DECLARE_LISTS(MusicSearchRecord)

/*! @brief The class of the search history Config.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SEARCH_EXPORT MusicLocalSongSearchRecordConfigManager : public MusicAbstractXml
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLocalSongSearchRecordConfigManager)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLocalSongSearchRecordConfigManager(QObject *parent = nullptr);

    /*!
     * Read history search datas from xml file by given name.
     */
    inline bool readConfig() { return MusicAbstractXml::readConfig(MUSICSEARCH_FULL); }

    /*!
     * Read datas from config file.
     */
    void readSearchData(MusicSearchRecords &records);
    /*!
     * Write datas into config file.
     */
    void writeSearchData(const MusicSearchRecords &records);

};

#endif // MUSICLOCALSONGSEARCHRECORDCONFIGMANAGER_H
