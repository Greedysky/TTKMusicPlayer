#ifndef MUSICUSERMODEL_H
#define MUSICUSERMODEL_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlQuery>
#include "musiclibexportglobal.h"

class MUSIC_USER_EXPORT MusicUserModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit MusicUserModel(QObject *parent = 0,
                            QSqlDatabase db = QSqlDatabase::database("user-data"));

    bool addUser(const QString &uid, const QString &pwd,
                 const QString &mail);
    bool updateUser(const QString &uid, const QString &pwd,
                    const QString &mail, const QString &name,
                    const QString &time);
    bool updateUser(const QString &uid, const QString &name,
                    const QString &sex, const QString &birth,
                    const QString &city, const QString &country,
                    const QString &sign);
    bool updateUserIcon(const QString &uid, const QString &icon);
    bool updateUserPwd(const QString &uid, const QString &pwd);

    bool deleteUser(const QString &uid);
    bool passwordCheck(const QString &uid, const QString &pwd);
    bool mailCheck(const QString &uid, const QString &mail);

    inline QString getUserLogTime(const QString &uid) { return getRecordData(uid, "LOGINTIME");}
    inline QString getUserName(const QString &uid) { return getRecordData(uid, "USERNAME");}
    inline QString getUserSex(const QString &uid) { return getRecordData(uid, "SEX");}
    inline QString getUserCity(const QString &uid) { return getRecordData(uid, "CITY");}
    inline QString getUserCountry(const QString &uid) { return getRecordData(uid, "COUNTRY");}
    inline QString getUserBirthday(const QString &uid) { return getRecordData(uid, "BIRTHDAY");}
    inline QString getUserSignature(const QString &uid) { return getRecordData(uid, "SIGNATURE");}
    inline QString getUserPWDMD5(const QString &uid) { return getRecordData(uid, "PASSWD");}
    inline QString getUserIcon(const QString &uid)
    {
        QString icon = getRecordData(uid, "ICON");
        return icon.isEmpty() ? ":/share/defaultArt" : icon;
    }

    QStringList getAllUsers();
    QStringList getAllCities();
    QStringList getAllCounties(const QString &city);

protected:
    QString userPasswordEncryption(const QString &pwd) const;
    QString getRecordData(const QString &uid, const QString &field);
    bool updateRecordData(const QString &uid, const QVariantMap &data);
    bool databaseSelectedFilter(const QString &uid);

};

#endif // MUSICUSERMODEL_H
