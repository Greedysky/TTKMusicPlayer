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
    bool checkUser(const QString &uid, const QString &pwd);
    bool deleteUser(const QString &uid);
    bool mailCheck(const QString &uid, const QString &mail);

    QStringList getAllUsers() const;
    QString getUserLogTime(const QString &uid);
    QString getUserName(const QString &uid);
    QString getUserPWDMD5(const QString &uid);
    bool databaseSelectedFilter(const QString &uid);

protected:
    QString userPasswordEncryption(const QString &pwd) const;

};

#endif // MUSICUSERMODEL_H
