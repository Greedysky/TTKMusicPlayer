#ifndef MUSICUSERMODEL_H
#define MUSICUSERMODEL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

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
    explicit MusicUserModel(QObject *parent = 0,
                            QSqlDatabase db = QSqlDatabase::database("user-data"));
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    bool addUser(const QString &uid, const QString &pwd,
                 const QString &mail);
    /*!
     * Insert user into databse by uid and pwd and mail.
     */
    bool updateUser(const QString &uid, const QString &pwd,
                    const QString &mail, const QString &name,
                    const QString &time);
    /*!
     * Update user into databse by uid and pwd and mail and name and logintime.
     */
    bool updateUser(const QString &uid, const QString &name,
                    const QString &sex, const QString &birth,
                    const QString &city, const QString &country,
                    const QString &sign);
    /*!
     * Update user into databse by uid and name and sex and
     * birth and city and country nad sign.
     */
    bool updateUserIcon(const QString &uid, const QString &icon);
    /*!
     * Update user icon by uid and icon path.
     */
    bool updateUserPwd(const QString &uid, const QString &pwd);
    /*!
     * Update user password by uid and new password.
     */

    bool deleteUser(const QString &uid);
    /*!
     * Delete user by given uid.
     */
    bool passwordCheck(const QString &uid, const QString &pwd);
    /*!
     * Check password valid or invalid by given uid and password.
     */
    bool mailCheck(const QString &uid, const QString &mail);
    /*!
     * Check mail exsit or not by given uid and mail.
     */

    inline QString getUserLogTime(const QString &uid) { return getRecordData(uid, "LOGINTIME");}
    /*!
     * Get user logintime.
     */
    inline QString getUserName(const QString &uid) { return getRecordData(uid, "USERNAME");}
    /*!
     * Get user name.
     */
    inline QString getUserSex(const QString &uid) { return getRecordData(uid, "SEX");}
    /*!
     * Get user sex.
     */
    inline QString getUserCity(const QString &uid) { return getRecordData(uid, "CITY");}
    /*!
     * Get user city.
     */
    inline QString getUserCountry(const QString &uid) { return getRecordData(uid, "COUNTRY");}
    /*!
     * Get user country.
     */
    inline QString getUserBirthday(const QString &uid) { return getRecordData(uid, "BIRTHDAY");}
    /*!
     * Get user birthday.
     */
    inline QString getUserSignature(const QString &uid) { return getRecordData(uid, "SIGNATURE");}
    /*!
     * Get user signature.
     */
    inline QString getUserPWDMD5(const QString &uid) { return getRecordData(uid, "PASSWD");}
    /*!
     * Get user password by md5 mut.
     */
    inline QString getUserIcon(const QString &uid)
    {
        QString icon = getRecordData(uid, "ICON");
        return icon.isEmpty()  || !QFile::exists(icon) ? ":/image/lb_defaultArt" : icon;
    }
    /*!
     * Get user icon.
     */

    QStringList getAllUsers();
    /*!
     * Get all user ids.
     */
    QStringList getAllCities();
    /*!
     * Get all cities.
     */
    QStringList getAllCountries(const QString &city);
    /*!
     * Get all countries.
     */

protected:
    QString userPasswordEncryption(const QString &pwd) const;
    /*!
     * To encrypt user password by md5.
     */
    QString getRecordData(const QString &uid, const QString &field);
    /*!
     * Get record data by uid and field.
     */
    bool updateRecordData(const QString &uid, const MusicObject::MStriantMap &data);
    /*!
     * Update record data by uid and field and data.
     */
    bool databaseSelectedFilter(const QString &uid);
    /*!
     * Set current database table by given uid.
     */

};

#endif // MUSICUSERMODEL_H
