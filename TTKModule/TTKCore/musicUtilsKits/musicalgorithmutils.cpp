#include "musicalgorithmutils.h"
#include "ttkcryptographichash.h"

#include <QCryptographicHash>

QByteArray TTK::Algorithm::md5(const QByteArray &data, bool base64)
{
    const QByteArray &v = QCryptographicHash::hash(data, QCryptographicHash::Md5);
    return base64 ? v.toBase64() : v.toHex().toLower();
}

QByteArray TTK::Algorithm::sha1(const QByteArray &data)
{
    return QCryptographicHash::hash(data, QCryptographicHash::Sha1);
}

QByteArray TTK::Algorithm::hmacSha1(const QByteArray &data, const QByteArray &key)
{
    const int blockSize = 64;
    QByteArray newSecretKey = key;
    if(newSecretKey.length() > blockSize)
    {
        newSecretKey = TTK::Algorithm::sha1(newSecretKey);
    }

    QByteArray innerPadding(blockSize, char(0x36));
    QByteArray outerPadding(blockSize, char(0x5C));

    for(int i = 0; i < key.length(); ++i)
    {
        innerPadding[i] = innerPadding[i] ^ key.at(i);
        outerPadding[i] = outerPadding[i] ^ key.at(i);
    }

    QByteArray total = outerPadding;
    QByteArray part = innerPadding;
    part.append(data);
    total.append(TTK::Algorithm::sha1(part));
    return TTK::Algorithm::sha1(total);
}

QString TTK::Algorithm::mdII(const QString &data, bool encode)
{
    TTKCryptographicHash hash;
    return encode ? hash.encrypt(data, MDII_URL_KEY) : hash.decrypt(data, MDII_URL_KEY);
}

QString TTK::Algorithm::mdII(const QString &data, const char *key, bool encode)
{
    TTKCryptographicHash hash;
    return encode ? hash.encrypt(data, key) : hash.decrypt(data, key);
}
