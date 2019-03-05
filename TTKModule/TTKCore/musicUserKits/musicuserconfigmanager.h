#ifndef MUSICXMLCONFIGMANAGER_H
#define MUSICXMLCONFIGMANAGER_H

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

/*! @brief The class of the user record item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_USER_EXPORT MusicUserRecord
{
    QString m_uid;
    QString m_password;
    int m_server;
    bool m_rememberFlag;
    bool m_autoFlag;

    MusicUserRecord()
    {
        m_server = 0;
        m_rememberFlag = false;
        m_autoFlag = false;
    }
}MusicUserRecord;
TTK_DECLARE_LISTS(MusicUserRecord)

/*! @brief The class of the user config manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_USER_EXPORT MusicUserConfigManager : public MusicAbstractXml
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicUserConfigManager)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicUserConfigManager(QObject *parent = nullptr);

    /*!
     * Read user datas from xml file by given name.
     */
    inline bool readConfig() { return MusicAbstractXml::readConfig(USERPATH_FULL); }

    /*!
     * Read datas from config file.
     */
    void readUserData(MusicUserRecords &records);
    /*!
     * Write datas into config file.
     */
    void writeUserData(const MusicUserRecords &records);

};

#endif // MUSICXMLCONFIGMANAGER_H
