#include "musicalgorithmutils.h"
#include "musiccryptographichash.h"

#include <QCryptographicHash>

QByteArray MusicUtils::Algorithm::md5(const QByteArray &data)
{
    return QCryptographicHash::hash(data, QCryptographicHash::Md5);
}

QByteArray MusicUtils::Algorithm::sha1(const QByteArray &data)
{
    return QCryptographicHash::hash(data, QCryptographicHash::Sha1);
}

QByteArray MusicUtils::Algorithm::hmacSha1(const QByteArray &data, const QByteArray &key)
{
    int blockSize = 64;
    QByteArray newSecretKey = key;
    if(newSecretKey.length() > blockSize)
    {
        newSecretKey = MusicUtils::Algorithm::sha1(newSecretKey);
    }

    QByteArray innerPadding(blockSize, char(0x36));
    QByteArray outerPadding(blockSize, char(0x5c));

    for(int i = 0; i < key.length(); i++)
    {
        innerPadding[i] = innerPadding[i] ^ key.at(i);
        outerPadding[i] = outerPadding[i] ^ key.at(i);
    }

    QByteArray total = outerPadding;
    QByteArray part = innerPadding;
    part.append(data);
    total.append(MusicUtils::Algorithm::sha1(part));
    QByteArray hashed = MusicUtils::Algorithm::sha1(total);
    return hashed;
}

void MusicUtils::Algorithm::urlEncode(QString &data)
{
    data.replace('+', "%2B");
    data.replace('/', "%2F");
    data.replace('=', "%3D");
}

void MusicUtils::Algorithm::urlDecode(QString &data)
{
    data.replace("%2B", "+");
    data.replace("%2F", "/");
    data.replace("%3D", "=");
}

void MusicUtils::Algorithm::urlEncode(QByteArray &data)
{
    data.replace('+', "%2B");
    data.replace('/', "%2F");
    data.replace('=', "%3D");
}

void MusicUtils::Algorithm::urlDecode(QByteArray &data)
{
    data.replace("%2B", "+");
    data.replace("%2F", "/");
    data.replace("%3D", "=");
}

QString MusicUtils::Algorithm::mdII(const QString &data, bool encode)
{
    MusicCryptographicHash hash;
    return encode ? hash.encrypt(data, ALG_URL_KEY) : hash.decrypt(data, ALG_URL_KEY);
}

QString MusicUtils::Algorithm::mdII(const QString &data, const QString &key, bool encode)
{
    MusicCryptographicHash hash;
    return encode ? hash.encrypt(data, key) : hash.decrypt(data, key);
}
