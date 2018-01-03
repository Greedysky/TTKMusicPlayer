#ifndef MUSICUSERMODEL_H
#define MUSICUSERMODEL_H

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

#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlQuery>

#include "musicobject.h"
#include "musicglobaldefine.h"

/*! @brief The class of the user data model.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_USER_EXPORT MusicUserModel : public QSqlTableModel
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicUserModel(QObject *parent = 0,
                            QSqlDatabase db = QSqlDatabase::database("user-data"));

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Insert user into databse by uid and pwd and mail.
     */
    bool addUser(const QString &uid, const QString &pwd,
                 const QString &mail);
    /*!
     * Update user into databse by uid and pwd and mail and name and logintime.
     */
    bool updateUser(const QString &uid, const QString &pwd,
                    const QString &mail, const QString &name,
                    const QString &time);
    /*!
     * Update user into databse by uid and name and sex and
     * birth and city and country nad sign.
     */
    bool updateUser(const QString &uid, const QString &name,
                    const QString &sex, const QString &birth,
                    const QString &city, const QString &country,
                    const QString &sign);
    /*!
     * Update user icon by uid and icon path.
     */
    bool updateUserIcon(const QString &uid, const QString &icon);
    /*!
     * Update user password by uid and new password.
     */
    bool updateUserPwd(const QString &uid, const QString &pwd);

    /*!
     * Delete user by given uid.
     */
    bool deleteUser(const QString &uid);
    /*!
     * Check password valid or invalid by given uid and password.
     */
    bool passwordCheck(const QString &uid, const QString &pwd);
    /*!
     * Check mail exsit or not by given uid and mail.
     */
    bool mailCheck(const QString &uid, const QString &mail);

    /*!
     * Get user logintime.
     */
    inline QString getUserLogTime(const QString &uid) { return getRecordData(uid, "LOGINTIME");}
    /*!
     * Get user name.
     */
    inline QString getUserName(const QString &uid) { return getRecordData(uid, "USERNAME");}
    /*!
     * Get user sex.
     */
    inline QString getUserSex(const QString &uid) { return getRecordData(uid, "SEX");}
    /*!
     * Get user city.
     */
    inline QString getUserCity(const QString &uid) { return getRecordData(uid, "CITY");}
    /*!
     * Get user country.
     */
    inline QString getUserCountry(const QString &uid) { return getRecordData(uid, "COUNTRY");}
    /*!
     * Get user birthday.
     */
    inline QString getUserBirthday(const QString &uid) { return getRecordData(uid, "BIRTHDAY");}
    /*!
     * Get user signature.
     */
    inline QString getUserSignature(const QString &uid) { return getRecordData(uid, "SIGNATURE");}
    /*!
     * Get user password by md5 mut.
     */
    inline QString getUserPWDMD5(const QString &uid) { return getRecordData(uid, "PASSWD");}
    /*!
     * Get user icon.
     */
    inline QString getUserIcon(const QString &uid)
    {
        QString icon = getRecordData(uid, "ICON");
        return icon.isEmpty()  || !QFile::exists(icon) ? ":/image/lb_defaultArt" : icon;
    }

    /*!
     * Get all user ids.
     */
    QStringList getAllUsers();
    /*!
     * Get all cities.
     */
    QStringList getAllCities();
    /*!
     * Get all countries.
     */
    QStringList getAllCountries(const QString &city);

protected:
    /*!
     * To encrypt user password by md5.
     */
    QString userPasswordEncryption(const QString &pwd) const;
    /*!
     * Get record data by uid and field.
     */
    QString getRecordData(const QString &uid, const QString &field);
    /*!
     * Update record data by uid and field and data.
     */
    bool updateRecordData(const QString &uid, const MusicObject::MStriantMap &data);
    /*!
     * Set current database table by given uid.
     */
    bool databaseSelectedFilter(const QString &uid);

};

#endif // MUSICUSERMODEL_H
