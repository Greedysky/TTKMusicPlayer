#include "musicdownloadwyinterface.h"
#include "musiccryptographichash.h"
#include "musicnumberutils.h"

#include <QCryptographicHash>

void MusicDownLoadWYInterface::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                                          const QVariantMap &key, int bitrate)
{
    if(key.isEmpty())
    {
        return;
    }

    MusicObject::MusicSongAttribute attr;
    qlonglong dfsId = key.value("dfsId").toLongLong();
    attr.m_bitrate = bitrate;
    attr.m_format = key.value("extension").toString();
    attr.m_size = MusicUtils::Number::size2Label(key.value("size").toInt());
    attr.m_url = MusicCryptographicHash::decryptData(WY_SONG_PATH_URL, URL_KEY).arg(encryptedId(dfsId)).arg(dfsId);
    info->m_songAttrs.append(attr);
}

void MusicDownLoadWYInterface::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                                          const QVariantMap &key, const QString &quality, bool all)
{
    if(all)
    {
        readFromMusicSongAttribute(info, key["lMusic"].toMap(), MB_32);
        readFromMusicSongAttribute(info, key["bMusic"].toMap(), MB_128);
        readFromMusicSongAttribute(info, key["mMusic"].toMap(), MB_192);
        readFromMusicSongAttribute(info, key["hMusic"].toMap(), MB_320);
    }
    else
    {
        if(quality == QObject::tr("ST"))
        {
            readFromMusicSongAttribute(info, key["lMusic"].toMap(), MB_32);
        }
        else if(quality == QObject::tr("SD"))
        {
            readFromMusicSongAttribute(info, key["bMusic"].toMap(), MB_128);
        }
        else if(quality == QObject::tr("HQ"))
        {
            readFromMusicSongAttribute(info, key["mMusic"].toMap(), MB_192);
        }
        else if(quality == QObject::tr("SQ"))
        {
            readFromMusicSongAttribute(info, key["hMusic"].toMap(), MB_320);
        }
    }
}

QString MusicDownLoadWYInterface::encryptedId(qlonglong id)
{
    return encryptedId(QString::number(id));
}

QString MusicDownLoadWYInterface::encryptedId(const QString &string)
{
    QByteArray array1(WY_ENCRYPT_STRING);
    QByteArray array2 = string.toUtf8();
    int length = array1.length();
    for(int i=0; i<array2.length(); ++i)
    {
        array2[i] = array2[i]^array1[i%length];
    }

    QByteArray encodedData = QCryptographicHash::hash(array2, QCryptographicHash::Md5);
#if (defined MUSIC_GREATER_NEW && !defined MUSIC_NO_WINEXTRAS)
    encodedData = encodedData.toBase64(QByteArray::Base64UrlEncoding);
#else
    encodedData = encodedData.toBase64();
    encodedData.replace('+', '-');
    encodedData.replace('/', '_');
#endif
    return encodedData;
}
