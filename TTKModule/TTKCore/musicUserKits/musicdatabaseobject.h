#ifndef MUSICDATABASEOBJECT_H
#define MUSICDATABASEOBJECT_H

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

#include <QObject>
#include "musicglobaldefine.h"

#define DB_SQLITE_DATABASE      "QSQLITE"
#define DB_MYSQL_DATABASE       "QMYSQL"
#define DB_OCI_DATABASE         "QOCI"

//
#define DB_USERID               20
#define DB_PASSWD               100
#define DB_EMAIL                20
#define DB_USERNAME             20
#define DB_LOGINTIME            20
#define DB_SEX                  2
#define DB_SERVER               2
//


/*! @brief The class of the user uid item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_USER_EXPORT MusicUserUIDItem
{
    QString m_uid;
    int m_server;

    MusicUserUIDItem()
    {
        clear();
    }

    MusicUserUIDItem(const QString &uid, int server)
    {
        m_uid = uid;
        m_server = server;
    }

    void clear()
    {
        m_uid.clear();
        m_server = 0;
    }
}MusicUserUIDItem;


/*! @brief The class of the user info record item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_USER_EXPORT MusicUserInfoRecord
{
    MusicUserUIDItem m_item;
    QString m_userName;
    QString m_password;
    QString m_nickName;
    QString m_coverUrl;

    bool isEmpty() const
    {
        return m_nickName.isEmpty() && m_coverUrl.isEmpty();
    }

    void clear()
    {
        m_item.clear();
        m_userName.clear();
        m_password.clear();
        m_nickName.clear();
        m_coverUrl.clear();
    }
}MusicUserInfoRecord;


#endif // MUSICDATABASEOBJECT_H
