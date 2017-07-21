#include "musicdownloadwyinterface.h"
#include "musicsemaphoreloop.h"
#include "musicnumberutils.h"
#include "musicalgorithmutils.h"
#///QJson import
#include "qjson/parser.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QNetworkAccessManager>

void MusicDownLoadWYInterface::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info, const QString &id, int bitrate)
{
    QUrl musicUrl = MusicUtils::Algorithm::mdII(WY_SONG_INFO_URL, false).arg(bitrate*1000).arg(id);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    QNetworkAccessManager manager;
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value["code"].toInt() == 200 && value.contains("data"))
        {
            value = value["data"].toMap();
            if(value.isEmpty())
            {
                return;
            }

            MusicObject::MusicSongAttribute attr;
            attr.m_url = value["url"].toString();
            attr.m_bitrate = value["br"].toInt()/1000;
            if(attr.m_url.isEmpty() || info->m_songAttrs.contains(attr))
            {
                return;
            }

            attr.m_size = MusicUtils::Number::size2Label(value["size"].toInt());
            attr.m_format = value["type"].toString();
            info->m_songAttrs.append(attr);
        }
    }
}

void MusicDownLoadWYInterface::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                                          const QVariantMap &key, const QString &id, int bitrate)
{
    qlonglong dfsId = key.value("dfsId").toLongLong();
    if(key.isEmpty() || dfsId == 0)
    {
        readFromMusicSongAttribute(info, id, bitrate);
    }
    else
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_bitrate = bitrate;
        attr.m_format = key.value("extension").toString();
        attr.m_size = MusicUtils::Number::size2Label(key.value("size").toInt());
        attr.m_url = MusicUtils::Algorithm::mdII(WY_SONG_PATH_URL, false).arg(encryptedId(dfsId)).arg(dfsId);
        info->m_songAttrs.append(attr);
    }
}

void MusicDownLoadWYInterface::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                                          const QVariantMap &key, const QString &quality, bool all)
{
    const QString id = key["id"].toString();
    if(all)
    {
        readFromMusicSongAttribute(info, key["bMusic"].toMap(), id, MB_128);
        readFromMusicSongAttribute(info, key["mMusic"].toMap(), id,  MB_192);
        readFromMusicSongAttribute(info, key["hMusic"].toMap(), id,  MB_320);
//        readFromMusicSongAttribute(info, QVariantMap(), id,  MB_999);
    }
    else
    {
        if(quality == QObject::tr("SD"))
        {
            readFromMusicSongAttribute(info, key["bMusic"].toMap(), id,  MB_128);
        }
        else if(quality == QObject::tr("HQ"))
        {
            readFromMusicSongAttribute(info, key["mMusic"].toMap(), id,  MB_192);
        }
        else if(quality == QObject::tr("SQ"))
        {
            readFromMusicSongAttribute(info, key["hMusic"].toMap(), id,  MB_320);
        }
//        else if(quality == QObject::tr("CD"))
//        {
//            readFromMusicSongAttribute(info, QVariantMap(), id,  MB_999);
//        }
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

    QByteArray encodedData = MusicUtils::Algorithm::md5(array2);
#if (defined MUSIC_GREATER_NEW && !defined MUSIC_NO_WINEXTRAS)
    encodedData = encodedData.toBase64(QByteArray::Base64UrlEncoding);
#else
    encodedData = encodedData.toBase64();
    encodedData.replace('+', '-');
    encodedData.replace('/', '_');
#endif
    return encodedData;
}
