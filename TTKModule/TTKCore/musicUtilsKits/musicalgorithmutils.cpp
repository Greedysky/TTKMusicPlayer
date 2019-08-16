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
    const int blockSize = 64;
    QByteArray newSecretKey = key;
    if(newSecretKey.length() > blockSize)
    {
        newSecretKey = MusicUtils::Algorithm::sha1(newSecretKey);
    }

    QByteArray innerPadding(blockSize, char(0x36));
    QByteArray outerPadding(blockSize, char(0x5C));

    for(int i = 0; i < key.length(); i++)
    {
        innerPadding[i] = innerPadding[i] ^ key.at(i);
        outerPadding[i] = outerPadding[i] ^ key.at(i);
    }

    QByteArray total = outerPadding;
    QByteArray part = innerPadding;
    part.append(data);
    total.append(MusicUtils::Algorithm::sha1(part));
    return MusicUtils::Algorithm::sha1(total);
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
