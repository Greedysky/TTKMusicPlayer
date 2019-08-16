#include "musicdownloadqqinterface.h"
#include "musicnumberdefine.h"
#include "musicnumberutils.h"
#include "musicsemaphoreloop.h"
#include "musictime.h"
#include "musicalgorithmutils.h"
#include "musicnetworkabstract.h"
#///QJson import
#include "qjson/parser.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QNetworkAccessManager>

#define REFER_URL   "M25YVkpIeHVOaVFRY0k3dmFWOFJsOE1tU013ZWV0Sy8="

void MusicDownLoadQQInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QVariantMap &key, int bitrate)
{
    MusicTime::timeSRand();
    QString mid = key["strMediaMid"].toString();
    if(mid.isEmpty())
    {
        mid = key["songmid"].toString();
    }

    if(key["size128"].toULongLong() != 0 && bitrate == MB_128)
    {
        const QString &randKey = QString::number(qrand());
        const QString &vkey = getMusicKey(randKey);
        if(vkey.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicUtils::Algorithm::mdII(QQ_SONG_128_URL, false).arg(mid).arg(vkey).arg(randKey);
        attr.m_size = MusicUtils::Number::size2Label(key["size128"].toULongLong());
        attr.m_format = MP3_FILE_PREFIX;
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["sizeogg"].toULongLong() != 0 && bitrate == MB_192)
    {
        const QString &randKey = QString::number(qrand());
        const QString &vkey = getMusicKey(randKey);
        if(vkey.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicUtils::Algorithm::mdII(QQ_SONG_192_URL, false).arg(mid).arg(vkey).arg(randKey);
        attr.m_size = MusicUtils::Number::size2Label(key["sizeogg"].toULongLong());
        attr.m_format = OGG_FILE_PREFIX;
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["size320"].toULongLong() != 0 && bitrate == MB_320)
    {
        const QString &randKey = QString::number(qrand());
        const QString &vkey = getMusicKey(randKey);
        if(vkey.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicUtils::Algorithm::mdII(QQ_SONG_320_URL, false).arg(mid).arg(vkey).arg(randKey);
        attr.m_size = MusicUtils::Number::size2Label(key["size320"].toULongLong());
        attr.m_format = MP3_FILE_PREFIX;
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["sizeape"].toULongLong() != 0 && bitrate == MB_750)
    {
        const QString &randKey = QString::number(qrand());
        const QString &vkey = getMusicKey(randKey);
        if(vkey.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicUtils::Algorithm::mdII(QQ_SONG_APE_URL, false).arg(mid).arg(vkey).arg(randKey);
        attr.m_size = MusicUtils::Number::size2Label(key["sizeape"].toULongLong());
        attr.m_format = APE_FILE_PREFIX;
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["sizeflac"].toULongLong() != 0 && bitrate == MB_1000)
    {
        const QString &randKey = QString::number(qrand());
        const QString &vkey = getMusicKey(randKey);
        if(vkey.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicUtils::Algorithm::mdII(QQ_SONG_FLAC_URL, false).arg(mid).arg(vkey).arg(randKey);
        attr.m_size = MusicUtils::Number::size2Label(key["sizeflac"].toULongLong());
        attr.m_format = FLC_FILE_PREFIX;
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
}

void MusicDownLoadQQInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QVariantMap &key, const QString &quality, bool all)
{
    if(all)
    {
        readFromMusicSongAttribute(info, key, MB_128);
        readFromMusicSongAttribute(info, key, MB_192);
        readFromMusicSongAttribute(info, key, MB_320);
        readFromMusicSongAttribute(info, key, MB_750);
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
            readFromMusicSongAttribute(info, key, MB_750);
            readFromMusicSongAttribute(info, key, MB_1000);
        }
    }
}

void MusicDownLoadQQInterface::readFromMusicSongAttributePlus(MusicObject::MusicSongInformation *info, const QVariantMap &key, int bitrate)
{
    MusicTime::timeSRand();
    const QString &mid = key["media_mid"].toString();
    if(key["size_128mp3"].toULongLong() != 0 && bitrate == MB_128)
    {
        const QString &randKey = QString::number(qrand());
        const QString &vkey = getMusicKey(randKey);
        if(vkey.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicUtils::Algorithm::mdII(QQ_SONG_128_URL, false).arg(mid).arg(vkey).arg(randKey);
        attr.m_size = MusicUtils::Number::size2Label(key["size_128mp3"].toULongLong());
        attr.m_format = MP3_FILE_PREFIX;
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["size_192ogg"].toULongLong() != 0 && bitrate == MB_192)
    {
        const QString &randKey = QString::number(qrand());
        const QString &vkey = getMusicKey(randKey);
        if(vkey.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicUtils::Algorithm::mdII(QQ_SONG_192_URL, false).arg(mid).arg(vkey).arg(randKey);
        attr.m_size = MusicUtils::Number::size2Label(key["size_192ogg"].toULongLong());
        attr.m_format = OGG_FILE_PREFIX;
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["size_320mp3"].toULongLong() != 0 && bitrate == MB_320)
    {
        const QString &randKey = QString::number(qrand());
        const QString &vkey = getMusicKey(randKey);
        if(vkey.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicUtils::Algorithm::mdII(QQ_SONG_320_URL, false).arg(mid).arg(vkey).arg(randKey);
        attr.m_size = MusicUtils::Number::size2Label(key["size_320mp3"].toULongLong());
        attr.m_format = MP3_FILE_PREFIX;
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["size_ape"].toULongLong() != 0 && bitrate == MB_750)
    {
        const QString &randKey = QString::number(qrand());
        const QString &vkey = getMusicKey(randKey);
        if(vkey.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicUtils::Algorithm::mdII(QQ_SONG_APE_URL, false).arg(mid).arg(vkey).arg(randKey);
        attr.m_size = MusicUtils::Number::size2Label(key["size_ape"].toULongLong());
        attr.m_format = APE_FILE_PREFIX;
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["size_flac"].toULongLong() != 0 && bitrate == MB_1000)
    {
        const QString &randKey = QString::number(qrand());
        const QString &vkey = getMusicKey(randKey);
        if(vkey.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicUtils::Algorithm::mdII(QQ_SONG_FLAC_URL, false).arg(mid).arg(vkey).arg(randKey);
        attr.m_size = MusicUtils::Number::size2Label(key["size_flac"].toULongLong());
        attr.m_format = FLC_FILE_PREFIX;
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
}

void MusicDownLoadQQInterface::readFromMusicSongAttributePlus(MusicObject::MusicSongInformation *info, const QVariantMap &key)
{
    readFromMusicSongAttributePlus(info, key, MB_128);
    readFromMusicSongAttributePlus(info, key, MB_192);
    readFromMusicSongAttributePlus(info, key, MB_320);
    readFromMusicSongAttributePlus(info, key, MB_750);
    readFromMusicSongAttributePlus(info, key, MB_1000);
}

QString MusicDownLoadQQInterface::getMusicKey(const QString &time)
{
    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(QQ_SONG_KEY_URL, false).arg(time);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(REFER_URL, false).toUtf8());
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(QQ_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    QNetworkAccessManager manager;
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return QString();
    }

    const QByteArray &bytes = reply->readAll();

    QJson::Parser parser;
    bool ok;
    const QVariant &data = parser.parse(bytes, &ok);
    if(ok)
    {
        const QVariantMap &value = data.toMap();
        if(value.contains("code") && value["code"].toInt() == 0 && value.contains("key"))
        {
            return value["key"].toString();
        }
    }

    return QString();
}
