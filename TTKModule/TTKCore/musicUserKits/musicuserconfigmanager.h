#ifndef MUSICXMLCONFIGMANAGER_H
#define MUSICXMLCONFIGMANAGER_H

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

/*! @brief The class of the user record item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_USER_EXPORT MusicUserRecord
{
    QString m_userName;
    QString m_password;
    QString m_rememberPWD;
    QString m_autoLogin;
}MusicUserRecord;
MUSIC_DECLARE_LISTS(MusicUserRecord)

/*! @brief The class of the user config manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_USER_EXPORT MusicUserConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicUserConfigManager(QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Read user datas from xml file by given name.
     */
    inline bool readUserXMLConfig(){ return readConfig(USERPATH_FULL); }
    /*!
     * Write user datas into xml file.
     */
    void writeUserXMLConfig(const MusicUserRecords &records);
    /*!
     * Read user datas into xml file.
     */
    void readUserConfig(MusicUserRecords &records);

};

#endif // MUSICXMLCONFIGMANAGER_H
