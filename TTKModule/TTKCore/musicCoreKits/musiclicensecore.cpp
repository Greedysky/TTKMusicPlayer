#include "musiclicensecore.h"
#include "musicalgorithmutils.h"

#include <QStringList>
#include <QNetworkInterface>

#define VALUE0   "ds231'332c53sd3!3d50147562s21.^6"
#define VALUE1   ".,lflfgdfKfdgdf2031..s121&&%%##"

MusicLicenseCore::MusicLicenseCore(QObject *parent)
    : QObject(parent)
{

}

QString MusicLicenseCore::getEncodeKey(const QString &key)
{
    return splitString(key.toUtf8(), VALUE1);
}

bool MusicLicenseCore::checkLicense(const QString &str)
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

QString MusicLicenseCore::getCharacteristicString()
{
    const QList<QNetworkInterface> &nets = QNetworkInterface::allInterfaces();
    if(!nets.isEmpty())
    {
        const QString &adds = nets.first().hardwareAddress();
        return splitString(adds.toUtf8(), VALUE0);
    }
    return QString();
}

QString MusicLicenseCore::getCharacteristicStringNormal()
{
    const QList<QNetworkInterface> &nets = QNetworkInterface::allInterfaces();
    if(!nets.isEmpty())
    {
        const QString &adds = nets.first().hardwareAddress();
        return MusicUtils::Algorithm::hmacSha1(adds.toUtf8(), VALUE0).toHex().mid(15, 10);
    }
    return QString();
}

QStringList MusicLicenseCore::getCharacteristicStrings()
{
    QStringList data;
    foreach(const QNetworkInterface &nets, QNetworkInterface::allInterfaces())
    {
        data << splitString(nets.hardwareAddress().toUtf8(), VALUE0);
    }
    return data;
}

QString MusicLicenseCore::splitString(const QByteArray &data, const QByteArray &key)
{
    QString value = MusicUtils::Algorithm::hmacSha1(data, key).toHex().mid(10, 20);
    const int count = value.count();
    for(int i=1; i<count/5; ++i)
    {
        value.insert(count - 5*i, "-");
    }
    return value;
}
