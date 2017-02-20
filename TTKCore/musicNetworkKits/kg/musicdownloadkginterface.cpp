#include "musicdownloadkginterface.h"
#include "musiccryptographichash.h"
#include "musicnumberutils.h"
#include "musicsemaphoreloop.h"

#///QJson import
#include "qjson/parser.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QCryptographicHash>
#include <QNetworkAccessManager>

void MusicDownLoadKGInterface::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                                          const QString &hash, QNetworkAccessManager *manager)
{
    if(hash.isEmpty())
    {
        return;
    }

    QByteArray encodedData = QCryptographicHash::hash(QString("%1kgcloud").arg(hash).toUtf8(),
                                                      QCryptographicHash::Md5).toHex().toLower();
    QUrl musicUrl = MusicCryptographicHash::decryptData(KG_SONG_ATTR_URL, URL_KEY).arg(QString(encodedData)).arg(hash);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = manager->get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(!value.isEmpty() && !value.contains("error"))
        {
            MusicObject::MusicSongAttribute attr;
            attr.m_url = value["url"].toString();
            attr.m_size = MusicUtils::Number::size2Label(value["fileSize"].toInt());
            attr.m_format = value["extName"].toString();
            attr.m_bitrate = map2NormalBitrate(value["bitRate"].toInt()/1000);
            info->m_songAttrs.append(attr);
        }
    }
}

void MusicDownLoadKGInterface::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info, QNetworkAccessManager *manager,
                                                          const QVariantMap &key, const QString &quality, bool all)
{
    if(all)
    {
        readFromMusicSongAttribute(info, key["hash"].toString(), manager);
        readFromMusicSongAttribute(info, key["320hash"].toString(), manager);
        readFromMusicSongAttribute(info, key["sqhash"].toString(), manager);
    }
    else
    {
        if(quality == QObject::tr("SD"))
        {
            readFromMusicSongAttribute(info, key["hash"].toString(), manager);
        }
        else if(quality == QObject::tr("SQ"))
        {
            readFromMusicSongAttribute(info, key["320hash"].toString(), manager);
        }
        else if(quality == QObject::tr("CD"))
        {
            readFromMusicSongAttribute(info, key["sqhash"].toString(), manager);
        }
    }
}

void MusicDownLoadKGInterface::readFromMusicSongLrcAndPic(MusicObject::MusicSongInfomation *info,
                                                          const QString &hash, QNetworkAccessManager *manager)
{
    if(hash.isEmpty())
    {
        return;
    }

    QUrl musicUrl = MusicCryptographicHash::decryptData(KG_SONG_INFO_URL, URL_KEY).arg(hash);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = manager->get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(!value.isEmpty() && value["error"].toString().isEmpty())
        {
            info->m_artistId = QString::number(value["singerId"].toULongLong());
            info->m_smallPicUrl = value["imgUrl"].toString().replace("{size}", "480");
            info->m_lrcUrl = MusicCryptographicHash::decryptData(KG_SONG_LRC_URL, URL_KEY)
                                                    .arg(value["songName"].toString()).arg(hash)
                                                    .arg(value["timeLength"].toInt()*1000);
        }
    }
}

int MusicDownLoadKGInterface::map2NormalBitrate(int bitrate)
{
    if(bitrate > MB_0 && bitrate <= MB_64)
        return MB_32;
    else if(bitrate > MB_64 && bitrate < MB_128)
        return MB_128;
    else if(bitrate > MB_128 && bitrate < MB_192)
        return MB_192;
    else if(bitrate > MB_192 && bitrate < MB_320)
        return MB_320;
    else if(bitrate > MB_320)
        return MB_500;
    else
        return bitrate;
}
