#include "qsyncutils.h"
#include "qsyncconfig.h"

#include <QLocale>
#include <QDateTime>
#include <QCryptographicHash>

static QString hmacSha1(QByteArray key, const QByteArray &baseString)
{
    int blockSize = 64; // HMAC-SHA-1 block size, defined in SHA-1 standard
    if(key.length() > blockSize)
    { // if key is longer than block size (64), reduce key length with SHA-1 compression
        key = QCryptographicHash::hash(key, QCryptographicHash::Sha1);
    }

    QByteArray innerPadding(blockSize, char(0x36)); // initialize inner padding with char "6"
    QByteArray outerPadding(blockSize, char(0x5c)); // initialize outer padding with char "\"
    // ascii characters 0x36 ("6") and 0x5c ("\") are selected because they have large
    // Hamming distance (http://en.wikipedia.org/wiki/Hamming_distance)

    for(int i = 0; i < key.length(); ++i)
    {
        innerPadding[i] = innerPadding[i] ^ key.at(i); // XOR operation between every byte in key and innerpadding, of key length
        outerPadding[i] = outerPadding[i] ^ key.at(i); // XOR operation between every byte in key and outerpadding, of key length
    }

    QByteArray total = outerPadding;
    QByteArray part = innerPadding;
    part.append(baseString);
    total.append(QCryptographicHash::hash(part, QCryptographicHash::Sha1));

    return QCryptographicHash::hash(total, QCryptographicHash::Sha1).toBase64();
}

static const char *self_define_header_prefix = "x-oss-";

static TTKStringMap formatHeader(const TTKStringMap &headers)
{
    TTKStringMap value;
    for(auto it = headers.constBegin(); it != headers.constEnd(); ++it)
    {
        if(it.key().toLower().startsWith(self_define_header_prefix))
        {
            value.insert(it.key().toLower(), it.value());
        }
        else
        {
            value.insert(it.key(), it.value());
        }
    }
    return value;
}

QString QSyncUtils::authorizationCode(const QString &key, const QString &method, const TTKStringMap &headers, const QString &resource)
{
    const QString &content_md5 = headers.value("Content-Md5");
    const QString &content_type = headers.value("Content-Type");
    const QString &date = headers.value("Date");
    const QString &canonicalized_resource = resource;

    QString canonicalized_headers;
    TTKStringMap origin_headers = formatHeader(headers);

    if(origin_headers.count() > 0)
    {
        for(auto it = origin_headers.constBegin(); it != origin_headers.constEnd(); ++it)
        {
            if(it.key().startsWith(self_define_header_prefix))
            {
                canonicalized_headers += it.key() + ":" + it.value() + "\n";
            }
        }
    }

    const QString &sign = method + "\n" + content_md5 + "\n" + content_type + "\n" + date + "\n" + canonicalized_headers + canonicalized_resource;
    return hmacSha1(key.toUtf8(), sign.toUtf8());
}

QString QSyncUtils::createSignForNormalAuth(const QString &method,  const QString &access, const QString &secret, const TTKStringMap &headers, const QString &resource)
{
    return QString("OSS ") + access + ":" + authorizationCode(secret,  method, headers, resource);
}

QString QSyncUtils::GMT()
{
    QLocale local(QLocale::English, QLocale::UnitedStates);
    return local.toString(QDateTime::currentDateTimeUtc(), "ddd, dd MMM yyyy hh:mm:ss") + " GMT";
}

QString QSyncUtils::makeDataBucketUrl()
{
    return QString("%1%2.%3/").arg(HTTP_PROTOCOL, SYNC_DATA_BUCKET, QSyncConfig::HOST);
}
