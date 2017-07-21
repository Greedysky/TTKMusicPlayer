#include "musicdownloadqqinterface.h"
#include "musicnumberdefine.h"
#include "musicnumberutils.h"
#include "musicsemaphoreloop.h"
#include "musictime.h"
#include "musicalgorithmutils.h"
#///QJson import
#include "qjson/parser.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QNetworkAccessManager>

void MusicDownLoadQQInterface::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
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
        QString vkey = getMusicKey(randKey);
        if(vkey.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicUtils::Algorithm::mdII(QQ_SONG_128_URL, false).arg(mid).arg(vkey).arg(randKey);
        attr.m_size = MusicUtils::Number::size2Label(key["size128"].toULongLong());
        attr.m_format = "mp3";
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["sizeogg"].toULongLong() != 0 && bitrate == MB_192)
    {
        QString randKey = QString::number(qrand());
        QString vkey = getMusicKey(randKey);
        if(vkey.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicUtils::Algorithm::mdII(QQ_SONG_192_URL, false).arg(mid).arg(vkey).arg(randKey);
        attr.m_size = MusicUtils::Number::size2Label(key["sizeogg"].toULongLong());
        attr.m_format = "ogg";
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["size320"].toULongLong() != 0 && bitrate == MB_320)
    {
        QString randKey = QString::number(qrand());
        QString vkey = getMusicKey(randKey);
        if(vkey.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicUtils::Algorithm::mdII(QQ_SONG_320_URL, false).arg(mid).arg(vkey).arg(randKey);
        attr.m_size = MusicUtils::Number::size2Label(key["size320"].toULongLong());
        attr.m_format = "mp3";
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["sizeape"].toULongLong() != 0 && bitrate == MB_500)
    {
        QString randKey = QString::number(qrand());
        QString vkey = getMusicKey(randKey);
        if(vkey.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicUtils::Algorithm::mdII(QQ_SONG_APE_URL, false).arg(mid).arg(vkey).arg(randKey);
        attr.m_size = MusicUtils::Number::size2Label(key["sizeape"].toULongLong());
        attr.m_format = "ape";
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["sizeflac"].toULongLong() != 0 && bitrate == MB_1000)
    {
        QString randKey = QString::number(qrand());
        QString vkey = getMusicKey(randKey);
        if(vkey.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicUtils::Algorithm::mdII(QQ_SONG_FLAC_URL, false).arg(mid).arg(vkey).arg(randKey);
        attr.m_size = MusicUtils::Number::size2Label(key["sizeflac"].toULongLong());
        attr.m_format = "flac";
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
}

void MusicDownLoadQQInterface::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info, const QVariantMap &key,
                                                          const QString &quality, bool all)
{
    if(all)
    {
        readFromMusicSongAttribute(info, key, MB_128);
        readFromMusicSongAttribute(info, key, MB_192);
        readFromMusicSongAttribute(info, key, MB_320);
        readFromMusicSongAttribute(info, key, MB_500);
        readFromMusicSongAttribute(info, key, MB_1000);
    }
    else
    {
        if(quality == QObject::tr("SD"))
        {
            readFromMusicSongAttribute(info, key, MB_128);
        }
        else if(quality == QObject::tr("HQ"))
        {
            readFromMusicSongAttribute(info, key, MB_192);
        }
        else if(quality == QObject::tr("SQ"))
        {
            readFromMusicSongAttribute(info, key, MB_320);
        }
        else if(quality == QObject::tr("CD"))
        {
            readFromMusicSongAttribute(info, key, MB_500);
            readFromMusicSongAttribute(info, key, MB_1000);
        }
    }
}

QString MusicDownLoadQQInterface::getMusicKey(const QString &time)
{
    QUrl musicUrl = MusicUtils::Algorithm::mdII(QQ_SONG_KEY_URL, false).arg(time);

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
