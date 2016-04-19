#include "musicusermodel.h"

#include <QStringList>
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
        M_LOGGER_INFO("submit successfully");
        return true;
    }
    else
    {
        M_LOGGER_INFO("submit failed");
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

QString MusicUserModel::getRecordData(const QString &uid, const QString &field)
{
    if(databaseSelectedFilter(uid))
    {
        return QString();
    }
    return record(0).value(field).toString();
}

bool MusicUserModel::updateRecordData(const QString &uid, const QVariantMap &data)
{
    if(databaseSelectedFilter(uid))
    {
        return false;
    }

    QStringList keys = data.keys();
    foreach(QString key, keys)
    {
        QString var = data[key].toString();
        if(!var.isEmpty())
        {
            setData(index(0, fieldIndex(key)), var);
        }
    }

    return submitAll();
}

bool MusicUserModel::updateUser(const QString &uid, const QString &pwd,
                                const QString &mail,const QString &name,
                                const QString &time)
{
    QVariantMap map;
    map["USERNAME"] = name;
    map["PASSWD"] = pwd.isEmpty() ? QString() : userPasswordEncryption(pwd);
    map["EMAIL"] = mail;
    map["LOGINTIME"] = time;
    return updateRecordData(uid, map);
}

bool MusicUserModel::updateUser(const QString &uid, const QString &name,
                                const QString &sex, const QString &birth,
                                const QString &city, const QString &country,
                                const QString &sign)
{
    QVariantMap map;
    map["USERNAME"] = name;
    map["SEX"] = sex;
    map["BIRTHDAY"] = birth;
    map["CITY"] = city;
    map["COUNTRY"] = country;
    map["SIGNATURE"] = sign;
    return updateRecordData(uid, map);
}

bool MusicUserModel::updateUserIcon(const QString &uid, const QString &icon)
{
    QVariantMap map;
    map["ICON"] = icon;
    return updateRecordData(uid, map);
}

bool MusicUserModel::updateUserPwd(const QString &uid, const QString &pwd)
{
    QVariantMap map;
    map["PASSWD"] = pwd.isEmpty() ? QString() : userPasswordEncryption(pwd);
    return updateRecordData(uid, map);
}

bool MusicUserModel::passwordCheck(const QString &uid, const QString &pwd)
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

QString MusicUserModel::userPasswordEncryption(const QString &pwd) const
{
    return QCryptographicHash::hash( pwd.toLatin1(), QCryptographicHash::Md5).toHex();
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

QStringList MusicUserModel::getAllCountries(const QString &city)
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
