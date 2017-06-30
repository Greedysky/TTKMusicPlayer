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
