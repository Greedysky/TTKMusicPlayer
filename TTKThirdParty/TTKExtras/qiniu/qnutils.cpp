#include "qnutils.h"

#include <QUrl>
#include <QDateTime>
#include <QStringList>
#include <QCryptographicHash>

const char QNUtils::KEY_ESCAPE_UNRESERVE_BYTES[] = {'!', '*', '\'', '(', ')', ';', ':', '@',
                                                    '&', '=', '+', '$', ',', '/', '#', '[', ']' };
const char QNUtils::KEY_ESCAPE_RESERVE_BYTES[] = {'?'};

// Url safe base64 encode
QString QNUtils::urlSafeBase64Encode(const QByteArray &data)
{
#if (defined TTK_GREATER_NEW && !defined TTK_NO_WINEXTRAS)
    const QByteArray &encodedData = data.toBase64(QByteArray::Base64UrlEncoding);
#else
    QByteArray encodedData = data.toBase64();
    encodedData.replace('+', '-');
    encodedData.replace('/', '_');
#endif
    return QString(encodedData);
}

// Url safe base64 decode
QByteArray QNUtils::urlSafeBase64Decode(const QString &data)
{
    QByteArray bytesToDecode = QByteArray(data.toLocal8Bit());
    bytesToDecode.replace('-', '+');
    bytesToDecode.replace('_', '/');
    return QByteArray::fromBase64(bytesToDecode);
}

// Sha1 hash
QByteArray QNUtils::sha1(const QByteArray &data)
{
    QCryptographicHash sha1Hash(QCryptographicHash::Sha1);
    sha1Hash.addData(data);
    return sha1Hash.result();
}

// Hmac sha1 hash using secret key
QByteArray QNUtils::hmacSha1(const QByteArray &data, const QByteArray &secretKey)
{
    const int blockSize = 64;
    QByteArray newSecretKey = secretKey;
    if(newSecretKey.length() > blockSize)
    {
        newSecretKey = QCryptographicHash::hash(newSecretKey, QCryptographicHash::Sha1);
    }

    QByteArray innerPadding(blockSize, char(0x36));
    QByteArray outerPadding(blockSize, char(0x5c));

    for(int i = 0; i < secretKey.length(); i++)
    {
        innerPadding[i] = innerPadding[i] ^ secretKey.at(i);
        outerPadding[i] = outerPadding[i] ^ secretKey.at(i);
    }

    QByteArray total = outerPadding;
    QByteArray part = innerPadding;
    part.append(data);
    total.append(QCryptographicHash::hash(part, QCryptographicHash::Sha1));
    return QCryptographicHash::hash(total, QCryptographicHash::Sha1);
}

// Url escaped key
// @see http://kb.qiniu.com/52slk76w
// @see http://en.wikipedia.org/wiki/Percent-encoding
QString QNUtils::escapeKey(const QString &key)
{
    return QUrl::toPercentEncoding(key, QNUtils::KEY_ESCAPE_UNRESERVE_BYTES, QNUtils::KEY_ESCAPE_RESERVE_BYTES);
}

// Check QString not null and not empty
bool QNUtils::isParamValid(const QString &param)
{
    return !param.isEmpty();
}

// A quick way to get a deadline timestamp
uint QNUtils::expireInSeconds(const uint seconds)
{
    QDateTime now = QDateTime::currentDateTime();
    //qiniu server timezone is utc+8
///    now.setOffsetFromUtc(3600*8);
    //default time span is one hour
    now = now.addSecs(seconds);
    return now.toTime_t();
}

// Calculate the crc32 for data bytes
uint QNUtils::crc32(const QByteArray &data)
{
    QByteArray temp;

    unsigned short crc = 0xffff;
    unsigned short a,j,k;

    for(a=0; a<data.size(); a++)
    {
        crc = crc ^ ( (int)data[a]>=0 ? data[a] : (data[a]+256) );
        for(j=0; j<8; j++)
        {
            k = crc & 01;
            crc = crc >> 1;
            if (k == 0) continue;
            crc = crc ^ 0xA001;
        }
    }
    temp[1] = crc/256;
    temp[0] = crc%256;
    return temp.toUInt();
}

// Create a random file name
QString QNUtils::randomFileName()
{
    QDateTime now = QDateTime::currentDateTime();
///    now.setOffsetFromUtc(3600*8);
    QString randFileName("Qiniu_Random_File_");
    randFileName.append(now.toTime_t());
    return randFileName;
}

QString QNUtils::urlQuery(const QString &url)
{
    int count = 0;
    for(int i=0; i<url.count(); ++i)
    {
        if(url[i] == '?')
        {
            ++count;
        }
    }
    return (count == 1) ? url.split('?').last() : QString();
}
