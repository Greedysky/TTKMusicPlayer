#include "musicdownloadxminterface.h"
#include "musiccryptographichash.h"
#include "musicnumberutils.h"
#include "musicsemaphoreloop.h"

#///QJson import
#include "qjson/parser.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QNetworkAccessManager>

MusicObject::MusicSongInfomation MusicDownLoadXMInterface::startLostSong(QNetworkAccessManager *manager, const QString &song)
{
    QUrl musicUrl = MusicCryptographicHash::decryptData(XM_SONG_LOST_URL, URL_KEY).arg(song);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Origin", MusicCryptographicHash::decryptData(XM_BASE_URL, URL_KEY).toUtf8());
    request.setRawHeader("Referer", MusicCryptographicHash::decryptData(XM_BASE_URL, URL_KEY).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = manager->get( request );
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    MusicObject::MusicSongInfomation musicInfo;

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("data"))
        {
            value = value["data"].toMap();
            value = value["song"].toMap();

            musicInfo.m_singerName = value["singers"].toString();
            musicInfo.m_songName = value["song_name"].toString();
            musicInfo.m_timeLength = "-";

            musicInfo.m_songId = QString::number(value["song_id"].toULongLong());
            musicInfo.m_artistId = QString::number(value["artist_id"].toULongLong());
            musicInfo.m_smallPicUrl = value["logo"].toString().replace("_1.", "_4.");
            QString lrcUrl = value["lyric"].toString();
            if(!lrcUrl.endsWith("txt"))
            {
                musicInfo.m_lrcUrl = lrcUrl;
            }

            QVariantMap data;
            data["filePath"] = value["listen_file"].toString();
            data["fileSize"] = "-";
            data["format"] = "mp3";
            readFromMusicSongAttribute(&musicInfo, data, MB_128);
        }
    }

    return musicInfo;
}

void MusicDownLoadXMInterface::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                                          const QVariantMap &key, int bitrate)
{
    MusicObject::MusicSongAttribute attr;
    attr.m_url = key["filePath"].toString();
    attr.m_size = MusicUtils::Number::size2Label(key["fileSize"].toInt());
    attr.m_format = key["format"].toString();
    attr.m_bitrate = bitrate;
    info->m_songAttrs.append(attr);
}

void MusicDownLoadXMInterface::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                                          const QVariantMap &key, const QString &quality, bool all)
{
    int bitrate = map2NormalBitrate(key["rate"].toInt());
    if(all)
    {
        readFromMusicSongAttribute(info, key, bitrate);
    }
    else
    {
        if(quality == QObject::tr("ST") && bitrate == MB_32)
        {
            readFromMusicSongAttribute(info, key, MB_32);
        }
        else if(quality == QObject::tr("SD") && bitrate == MB_128)
        {
            readFromMusicSongAttribute(info, key, MB_128);
        }
        else if(quality == QObject::tr("HQ") && bitrate == MB_192)
        {
            readFromMusicSongAttribute(info, key, MB_192);
        }
        else if(quality == QObject::tr("SQ") && bitrate == MB_320)
        {
            readFromMusicSongAttribute(info, key, MB_320);
        }
        else if(quality == QObject::tr("CD") && bitrate == MB_500)
        {
            readFromMusicSongAttribute(info, key, MB_500);
        }
    }
}

int MusicDownLoadXMInterface::map2NormalBitrate(int bitrate)
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
