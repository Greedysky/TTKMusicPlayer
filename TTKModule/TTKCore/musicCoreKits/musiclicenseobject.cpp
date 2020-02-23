#include "musiclicenseobject.h"
#include "musicalgorithmutils.h"

#include <QStringList>
#include <QNetworkInterface>

#define VALUE0   "ds231'332c53sd3!3d50147562s21.^6"
#define VALUE1   ".,lflfgdfKfdgdf2031..s121&&%%##"

MusicLicenseObject::MusicLicenseObject(QObject *parent)
    : QObject(parent)
{

}

QString MusicLicenseObject::getEncodeKey(const QString &key)
{
    return stringSplit(key.toUtf8(), VALUE1);
}

bool MusicLicenseObject::checkLicense(const QString &str)
{
    foreach(const QString &key, getCharacteristicStrings())
    {
        if(str == getEncodeKey(key))
        {
            return true;
        }
    }
    return false;
}

QString MusicLicenseObject::getCharacteristicString()
{
    const QList<QNetworkInterface> &nets = QNetworkInterface::allInterfaces();
    if(!nets.isEmpty())
    {
        const QString &adds = nets.first().hardwareAddress();
        return stringSplit(adds.toUtf8(), VALUE0);
    }
    return QString();
}

QString MusicLicenseObject::getCharacteristicStringNormal()
{
    const QList<QNetworkInterface> &nets = QNetworkInterface::allInterfaces();
    if(!nets.isEmpty())
    {
        const QString &adds = nets.first().hardwareAddress();
        return MusicUtils::Algorithm::hmacSha1(adds.toUtf8(), VALUE0).toHex().mid(15, 10);
    }
    return QString();
}

QStringList MusicLicenseObject::getCharacteristicStrings()
{
    QStringList data;
    foreach(const QNetworkInterface &nets, QNetworkInterface::allInterfaces())
    {
        data << stringSplit(nets.hardwareAddress().toUtf8(), VALUE0);
    }
    return data;
}

QString MusicLicenseObject::stringSplit(const QByteArray &data, const QByteArray &key)
{
    QString value = MusicUtils::Algorithm::hmacSha1(data, key).toHex().mid(10, 20);
    const int count = value.count();
    for(int i=1; i<count/5; ++i)
    {
        value.insert(count - 5*i, "-");
    }
    return value;
}
