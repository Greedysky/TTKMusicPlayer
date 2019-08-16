#include "musicusermodel.h"
#include "musicalgorithmutils.h"

#include <QSet>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringList>

MusicUserModel::MusicUserModel(QObject *parent,QSqlDatabase db)
    : QSqlTableModel(parent,db)
{
    setEditStrategy(QSqlTableModel::OnManualSubmit);
}

bool MusicUserModel::addUser(const MusicUserUIDItem &uid, const QString &pwd, const QString &mail, bool pwdMask)
{
    if(!databaseSelectedFilter(uid))
    {
        return false;
    }

    setTable("MusicUser");
    select();

    insertRow(0);
    setData(index(0, fieldIndex("USERID")), uid.m_uid);
    setData(index(0, fieldIndex("SERVER")), uid.m_server);
    setData(index(0, fieldIndex("PASSWD")), pwdMask ? pwd : userPasswordEncryption(pwd));
    setData(index(0, fieldIndex("EMAIL")), mail);
    setData(index(0, fieldIndex("USERNAME")), uid.m_uid);
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

bool MusicUserModel::addUser(const MusicUserInfoRecord &info)
{
    if(!databaseSelectedFilter(info.m_item))
    {
        return false;
    }

    setTable("MusicUser");
    select();

    insertRow(0);
    setData(index(0, fieldIndex("USERID")), info.m_item.m_uid);
    setData(index(0, fieldIndex("SERVER")), info.m_item.m_server);
    setData(index(0, fieldIndex("PASSWD")), userPasswordEncryption(info.m_password));
    setData(index(0, fieldIndex("USERNAME")), info.m_nickName);
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

bool MusicUserModel::updateUser(const MusicUserUIDItem &uid, const QString &pwd, const QString &mail, const QString &name, const QString &time, bool pwdMask)
{
    MVariantMap map;
    map["USERNAME"] = name;
    map["PASSWD"] = pwd.isEmpty() ? QString() : (pwdMask ? pwd : userPasswordEncryption(pwd));
    map["EMAIL"] = mail;
    map["LOGINTIME"] = time;
    return updateRecordData(uid, map);
}

bool MusicUserModel::updateUser(const MusicUserUIDItem &uid, const QString &name, const QString &sex, const QString &birth, const QString &city, const QString &country, const QString &sign)
{
    MVariantMap map;
    map["USERNAME"] = name;
    map["SEX"] = sex;
    map["BIRTHDAY"] = birth;
    map["CITY"] = city;
    map["COUNTRY"] = country;
    map["SIGNATURE"] = sign;
    return updateRecordData(uid, map);
}

bool MusicUserModel::updateUser(const MusicUserInfoRecord &info)
{
    MVariantMap map;
    map["USERNAME"] = info.m_nickName;
    return updateRecordData(info.m_item, map);
}

bool MusicUserModel::updateUserIcon(const MusicUserUIDItem &uid, const QString &icon)
{
    MVariantMap map;
    map["ICON"] = icon;
    return updateRecordData(uid, map);
}

bool MusicUserModel::updateUserPwd(const MusicUserUIDItem &uid, const QString &pwd, bool pwdMask)
{
    MVariantMap map;
    map["PASSWD"] = pwd.isEmpty() ? QString() : (pwdMask ? pwd : userPasswordEncryption(pwd));
    return updateRecordData(uid, map);
}

bool MusicUserModel::deleteUser(const MusicUserUIDItem &uid)
{
    if(databaseSelectedFilter(uid))
    {
        return false;
    }

    removeRow(0);
    return submitAll();
}

bool MusicUserModel::passwordCheck(const MusicUserUIDItem &uid, const QString &pwd, bool pwdMask)
{
    if(databaseSelectedFilter(uid))
    {
        return false;
    }

    return record(0).value("PASSWD") == (pwdMask ? pwd : userPasswordEncryption(pwd));
}

bool MusicUserModel::mailCheck(const MusicUserUIDItem &uid, const QString &mail)
{
    if(databaseSelectedFilter(uid))
    {
        return false;
    }

    return record(0).value("EMAIL") == mail;
}

QString MusicUserModel::userPasswordEncryption(const QString &pwd) const
{
    return MusicUtils::Algorithm::md5( pwd.toUtf8() ).toHex();
}

QStringList MusicUserModel::getAllUsers()
{
    setTable("MusicUser");
    select();

    QSet<QString> users;
    for(int i=0; i<rowCount(); ++i)
    {
        users.insert(record(i).value("USERID").toString());
    }
    return users.toList();
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

QString MusicUserModel::getRecordData(const MusicUserUIDItem &uid, const QString &field)
{
    if(databaseSelectedFilter(uid))
    {
        return QString();
    }
    return record(0).value(field).toString();
}

bool MusicUserModel::updateRecordData(const MusicUserUIDItem &uid, const MVariantMap &data)
{
    if(databaseSelectedFilter(uid))
    {
        return false;
    }

    const QStringList &keys = data.keys();
    foreach(const QString &key, keys)
    {
        const QString &var = data[key].toString();
        if(!var.isEmpty())
        {
            setData(index(0, fieldIndex(key)), var);
        }
    }

    return submitAll();
}

bool MusicUserModel::databaseSelectedFilter(const MusicUserUIDItem &uid)
{
    setTable("MusicUser");
    setFilter(QString("USERID='%1' and SERVER='%2'").arg(uid.m_uid).arg(uid.m_server));
    select();
    return (rowCount() == 0);
}
