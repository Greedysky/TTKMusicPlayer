#include "musicdownloadqqinterface.h"
#include "musiccryptographichash.h"
#include "musicnumberdefine.h"
#include "musicnumberutils.h"
#include "musicsemaphoreloop.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QNetworkAccessManager>

void MusicDownLoadQQInterface::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info, QNetworkAccessManager *manager,
                                                          const QVariantMap &key, int bitrate)
{
    MusicTime::timeSRand();
    QString mid = key["strMediaMid"].toString();
    if(mid.isEmpty())
    {
        mid = key["songmid"].toString();
    }

    if(key["size128"].toULongLong() != 0 && bitrate == MB_128)
    {
        QString randKey = QString::number(qrand());
        QString vkey = getMusicKey(manager, randKey);

        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicCryptographicHash::decryptData(QQ_SONG_128_URL, URL_KEY).arg(mid).arg(vkey).arg(randKey);
        attr.m_size = MusicUtils::Number::size2Label(key["size128"].toULongLong());
        attr.m_format = "mp3";
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["sizeogg"].toULongLong() != 0 && bitrate == MB_192)
    {
        QString randKey = QString::number(qrand());
        QString vkey = getMusicKey(manager, randKey);

        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicCryptographicHash::decryptData(QQ_SONG_192_URL, URL_KEY).arg(mid).arg(vkey).arg(randKey);
        attr.m_size = MusicUtils::Number::size2Label(key["sizeogg"].toULongLong());
        attr.m_format = "ogg";
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["size320"].toULongLong() != 0 && bitrate == MB_320)
    {
        QString randKey = QString::number(qrand());
        QString vkey = getMusicKey(manager, randKey);

        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicCryptographicHash::decryptData(QQ_SONG_320_URL, URL_KEY).arg(mid).arg(vkey).arg(randKey);
        attr.m_size = MusicUtils::Number::size2Label(key["size320"].toULongLong());
        attr.m_format = "mp3";
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["sizeape"].toULongLong() != 0 && bitrate == MB_500)
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicCryptographicHash::decryptData(QQ_SONG_APE_URL, URL_KEY).arg(mid);
        attr.m_size = MusicUtils::Number::size2Label(key["sizeape"].toULongLong());
        attr.m_format = "ape";
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["sizeflac"].toULongLong() != 0 && bitrate == MB_1000)
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicCryptographicHash::decryptData(QQ_SONG_FLAC_URL, URL_KEY).arg(mid);
        attr.m_size = MusicUtils::Number::size2Label(key["sizeflac"].toULongLong());
        attr.m_format = "flac";
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
}

void MusicDownLoadQQInterface::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info, QNetworkAccessManager *manager,
                                                          const QVariantMap &key, const QString &quality, bool all)
{
    if(all)
    {
        readFromMusicSongAttribute(info, manager, key, MB_128);
        readFromMusicSongAttribute(info, manager, key, MB_192);
        readFromMusicSongAttribute(info, manager, key, MB_320);
        readFromMusicSongAttribute(info, manager, key, MB_500);
        readFromMusicSongAttribute(info, manager, key, MB_1000);
    }
    else
    {
        if(quality == QObject::tr("SD"))
        {
            readFromMusicSongAttribute(info, manager, key, MB_128);
        }
        else if(quality == QObject::tr("HQ"))
        {
            readFromMusicSongAttribute(info, manager, key, MB_192);
        }
        else if(quality == QObject::tr("SQ"))
        {
            readFromMusicSongAttribute(info, manager, key, MB_320);
        }
        else if(quality == QObject::tr("CD"))
        {
            readFromMusicSongAttribute(info, manager, key, MB_500);
            readFromMusicSongAttribute(info, manager, key, MB_1000);
        }
    }
}

QString MusicDownLoadQQInterface::getMusicKey(QNetworkAccessManager *manager, const QString &time)
{
    QUrl musicUrl = MusicCryptographicHash::decryptData(QQ_SONG_KEY_URL, URL_KEY).arg(time);

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

    QByteArray bytes = reply->readAll();
    bytes.replace("jsonCallback(", "");
    bytes.chop(2);

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("code") && value["code"].toInt() == 0 && value.contains("key"))
        {
            return value["key"].toString();
        }
    }

    return QString();
}
