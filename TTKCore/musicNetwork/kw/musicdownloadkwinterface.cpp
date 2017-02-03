#include "musicdownloadkwinterface.h"
#include "musiccryptographichash.h"
#include "musicnumberutils.h"
#include "musicsemaphoreloop.h"

#///QJson import
#include "qjson/parser.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

void MusicDownLoadKWInterface::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info, const QString &suffix,
                                                          const QString &format, const QString &id, int bitrate)
{
    if(format.contains("MP3128") && bitrate == MB_128 && suffix == "mp3")
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_bitrate = bitrate;
        attr.m_format = suffix;
        attr.m_size = "-";
        attr.m_url = MusicCryptographicHash::decryptData(KW_SONG_ATTR_URL, URL_KEY).arg(bitrate).arg(id);
        info->m_songAttrs.append(attr);
    }
    else if(format.contains("MP3192") && bitrate == MB_192 && suffix == "mp3")
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_bitrate = bitrate;
        attr.m_format = suffix;
        attr.m_size = "-";
        attr.m_url = MusicCryptographicHash::decryptData(KW_SONG_ATTR_URL, URL_KEY).arg(bitrate).arg(id);
        info->m_songAttrs.append(attr);
    }
    else if(format.contains("MP3H") && bitrate == MB_320 && suffix == "mp3")
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_bitrate = bitrate;
        attr.m_format = suffix;
        attr.m_size = "-";
        attr.m_url = MusicCryptographicHash::decryptData(KW_SONG_ATTR_URL, URL_KEY).arg(bitrate).arg(id);
        info->m_songAttrs.append(attr);
    }
    else if(format.contains("AL") && bitrate == MB_1000 && suffix == "ape")
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_bitrate = bitrate;
        attr.m_format = suffix;
        attr.m_size = "-";
        attr.m_url = MusicCryptographicHash::decryptData(KW_SONG_ATTRS_URL, URL_KEY).arg(id).arg(suffix);
        info->m_songAttrs.append(attr);
    }
    else if(format.contains("MP4") && bitrate == MB_1000 && suffix == "mp4")
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_bitrate = bitrate;
        attr.m_format = suffix;
        attr.m_size = "-";
        attr.m_url = MusicCryptographicHash::decryptData(KW_SONG_ATTRS_URL, URL_KEY).arg(id).arg(suffix);
        info->m_songAttrs.append(attr);
    }
    else if(format.contains("MV") && bitrate == MB_1000 && suffix == "mkv")
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_bitrate = bitrate;
        attr.m_format = suffix;
        attr.m_size = "-";
        attr.m_url = MusicCryptographicHash::decryptData(KW_SONG_ATTRS_URL, URL_KEY).arg(id).arg(suffix);
        info->m_songAttrs.append(attr);
    }
}

void MusicDownLoadKWInterface::readFromMusicSongPic(MusicObject::MusicSongInfomation *info,
                                                    const QString &id, QNetworkAccessManager *manager)
{
    if(id.isEmpty())
    {
        return;
    }

    QUrl musicUrl = MusicCryptographicHash::decryptData(KW_SONG_INFO_URL, URL_KEY).arg(id);

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
    QVariant data = parser.parse(reply->readAll().replace("lrclist", "'lrclist'").replace("'", "\""), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("songinfo"))
        {
            value = value["songinfo"].toMap();
            info->m_smallPicUrl = MusicCryptographicHash::decryptData(KW_ALBUM_COVER_URL, URL_KEY) + value["artpic"].toString();
        }
    }
}
