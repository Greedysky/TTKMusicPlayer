#ifndef MUSICUSERMODEL_H
#define MUSICUSERMODEL_H

#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlQuery>
#include "../core/musiclibexportglobal.h"

class MUSIC_EXPORT MusicUserModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit MusicUserModel(QObject *parent = 0,
                            QSqlDatabase db = QSqlDatabase::database("user-data"));

    bool addUser(const QString& uid, const QString& pwd,
                 const QString& mail);
    bool updateUser(const QString& uid, const QString& pwd,
                    const QString& mail, const QString& name,
                    const QString& time);
    bool checkUser(const QString& uid, const QString& pwd);
    bool deleteUser(const QString& uid);
    bool mailCheck(const QString& uid, const QString& mail);

    QStringList getAllUsers();
    QString getUserLogTime(const QString& uid);
    QString getUserName(const QString& uid);

    QString getUserPWDMD5(const QString& uid);

    bool databaseSelectedFilter(const QString&);

protected:
    QString userPasswordEncryption(const QString& pwd);

};

#endif // MUSICUSERMODEL_H
