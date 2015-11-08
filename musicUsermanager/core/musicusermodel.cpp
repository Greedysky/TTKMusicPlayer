#include "musicusermodel.h"

#include <QCryptographicHash>
#include <QtSql/QSqlRecord>
#include <QSet>

MusicUserModel::MusicUserModel(QObject *parent,QSqlDatabase db)
    : QSqlTableModel(parent,db)
{
    setEditStrategy(QSqlTableModel::OnManualSubmit);
}

bool MusicUserModel::addUser(const QString &uid, const QString &pwd,
                             const QString &mail)
{
    setTable("MusicUser");
    select();

    insertRow(0);
    setData(index(0, fieldIndex("USERID")), uid);
    setData(index(0, fieldIndex("PASSWD")), userPasswordEncryption(pwd));
    setData(index(0, fieldIndex("EMAIL")), mail);
    setData(index(0, fieldIndex("USERNAME")), uid);
    setData(index(0, fieldIndex("LOGINTIME")), 0);
    database().transaction();
    if(submitAll())
    {
        database().commit();
        M_LOGGER << "submit successfully" << LOG_END;
        return true;
    }
    else
    {
        M_LOGGER << "submit failed" << LOG_END;
        database().rollback();
        return false;
    }
}

bool MusicUserModel::databaseSelectedFilter(const QString &uid)
{
    setTable("MusicUser");
    setFilter(QString("USERID='%1'").arg(uid));
    select();
    return (rowCount() == 0);
}

bool MusicUserModel::updateUser(const QString &uid, const QString &pwd,
                                const QString &mail,const QString &name,
                                const QString &time)
{
    if(databaseSelectedFilter(uid))
    {
        return false;
    }

    if(!pwd.isEmpty()) setData(index(0, fieldIndex("PASSWD")), userPasswordEncryption(pwd));
    if(!mail.isEmpty()) setData(index(0, fieldIndex("EMAIL")), mail);
    if(!name.isEmpty()) setData(index(0, fieldIndex("USERNAME")), name);
    if(!time.isEmpty()) setData(index(0, fieldIndex("LOGINTIME")), time);
    submitAll();
    return true;
}

bool MusicUserModel::checkUser(const QString &uid, const QString &pwd)
{
    if(databaseSelectedFilter(uid))
    {
        return false;
    }

    return record(0).value("PASSWD") == userPasswordEncryption(pwd);
}

bool MusicUserModel::deleteUser(const QString &uid)
{
    if(databaseSelectedFilter(uid))
    {
        return false;
    }

    removeRow(0);
    return submitAll();
}

bool MusicUserModel::mailCheck(const QString &uid, const QString &mail)
{
    if(databaseSelectedFilter(uid))
    {
        return false;
    }

    return record(0).value("EMAIL") == mail;
}

QStringList MusicUserModel::getAllUsers()
{
    setTable("MusicUser");
    select();

    QStringList users;
    for(int i=0; i<rowCount(); ++i)
    {
        users << record(i).value("USERID").toString();
    }
    return users;
}

QString MusicUserModel::getUserLogTime(const QString &uid)
{
    if(databaseSelectedFilter(uid))
    {
        return QString();
    }

    return record(0).value("LOGINTIME").toString();
}

QString MusicUserModel::getUserName(const QString &uid)
{
    if(databaseSelectedFilter(uid))
    {
        return QString();
    }

    return record(0).value("USERNAME").toString();
}

QString MusicUserModel::getUserPWDMD5(const QString &uid)
{
    if(databaseSelectedFilter(uid))
    {
        return QString();
    }

    return record(0).value("PASSWD").toString();
}

QString MusicUserModel::userPasswordEncryption(const QString &pwd) const
{
    return QCryptographicHash::hash( pwd.toLatin1(),
                               QCryptographicHash::Sha256).toHex();
}

QStringList MusicUserModel::getAllCities()
{
    setTable("MusicCountry");
    select();

    QSet<QString> cities;
    for(int i=0; i<rowCount(); ++i)
    {
        cities << record(i).value("CITYNAME").toString();
    }

    return cities.toList();
}

QStringList MusicUserModel::getAllCounties(const QString &city)
{
    setTable("MusicCountry");
    setFilter(QString("CITYNAME='%1'").arg(city));
    select();

    QStringList cities;
    for(int i=0; i<rowCount(); ++i)
    {
        cities << record(i).value("COUNTRYNAME").toString();
    }

    return cities;
}
