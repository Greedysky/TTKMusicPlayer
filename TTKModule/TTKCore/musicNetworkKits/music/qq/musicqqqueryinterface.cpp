#include "musicqqqueryinterface.h"
#include "musicnumberutils.h"
#include "musicsemaphoreloop.h"
#include "musicalgorithmutils.h"
#include "musicabstractnetwork.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QNetworkAccessManager>

#define REFER_URL   "M25YVkpIeHVOaVFRY0k3dmFWOFJsOE1tU013ZWV0Sy8="

void MusicQQQueryInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QVariantMap &key, int bitrate)
{
    const QString &mid = key["songmid"].toString();
    if(key["size128"].toULongLong() != 0 && bitrate == MB_128)
    {
        const QString &musicUrl = getMusicPath("M500" + mid + MP3_FILE, mid);
        if(musicUrl.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongAttribute attr;
        attr.m_url = musicUrl;
        attr.m_size = MusicUtils::Number::size2Label(key["size128"].toULongLong());
        attr.m_format = MP3_FILE_PREFIX;
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["size320"].toULongLong() != 0 && bitrate == MB_320)
    {
        const QString &musicUrl = getMusicPath("M800" + mid + MP3_FILE, mid);
        if(musicUrl.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongAttribute attr;
        attr.m_url = musicUrl;
        attr.m_size = MusicUtils::Number::size2Label(key["size320"].toULongLong());
        attr.m_format = MP3_FILE_PREFIX;
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["sizeape"].toULongLong() != 0 && bitrate == MB_750)
    {
        const QString &musicUrl = getMusicPath("A000" + mid + APE_FILE, mid);
        if(musicUrl.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongAttribute attr;
        attr.m_url = musicUrl;
        attr.m_size = MusicUtils::Number::size2Label(key["sizeape"].toULongLong());
        attr.m_format = APE_FILE_PREFIX;
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["sizeflac"].toULongLong() != 0 && bitrate == MB_1000)
    {
        const QString &musicUrl = getMusicPath("F000" + mid + FLC_FILE, mid);
        if(musicUrl.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongAttribute attr;
        attr.m_url = musicUrl;
        attr.m_size = MusicUtils::Number::size2Label(key["sizeflac"].toULongLong());
        attr.m_format = FLC_FILE_PREFIX;
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
}

void MusicQQQueryInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QVariantMap &key, const QString &quality, bool all)
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

void MusicQQQueryInterface::readFromMusicSongAttributePlus(MusicObject::MusicSongInformation *info, const QVariantMap &key, int bitrate)
{
    const QString &mid = key["media_mid"].toString();
    if(key["size_128mp3"].toULongLong() != 0 && bitrate == MB_128)
    {
        const QString &musicUrl = getMusicPath("M500" + mid + MP3_FILE, mid);
        if(musicUrl.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongAttribute attr;
        attr.m_url = musicUrl;
        attr.m_size = MusicUtils::Number::size2Label(key["size_128mp3"].toULongLong());
        attr.m_format = MP3_FILE_PREFIX;
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["size_320mp3"].toULongLong() != 0 && bitrate == MB_320)
    {
        const QString &musicUrl = getMusicPath("M800" + mid + MP3_FILE, mid);
        if(musicUrl.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongAttribute attr;
        attr.m_url = musicUrl;
        attr.m_size = MusicUtils::Number::size2Label(key["size_320mp3"].toULongLong());
        attr.m_format = MP3_FILE_PREFIX;
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["size_ape"].toULongLong() != 0 && bitrate == MB_750)
    {
        const QString &musicUrl = getMusicPath("A000" + mid + APE_FILE, mid);
        if(musicUrl.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongAttribute attr;
        attr.m_url = musicUrl;
        attr.m_size = MusicUtils::Number::size2Label(key["size_ape"].toULongLong());
        attr.m_format = APE_FILE_PREFIX;
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["size_flac"].toULongLong() != 0 && bitrate == MB_1000)
    {
        const QString &musicUrl = getMusicPath("F000" + mid + FLC_FILE, mid);
        if(musicUrl.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongAttribute attr;
        attr.m_url = musicUrl;
        attr.m_size = MusicUtils::Number::size2Label(key["size_flac"].toULongLong());
        attr.m_format = FLC_FILE_PREFIX;
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
}

void MusicQQQueryInterface::readFromMusicSongAttributePlus(MusicObject::MusicSongInformation *info, const QVariantMap &key)
{
    readFromMusicSongAttributePlus(info, key, MB_128);
    readFromMusicSongAttributePlus(info, key, MB_192);
    readFromMusicSongAttributePlus(info, key, MB_320);
    readFromMusicSongAttributePlus(info, key, MB_750);
    readFromMusicSongAttributePlus(info, key, MB_1000);
}

QString MusicQQQueryInterface::getMusicPath(const QString &file, const QString &mid)
{
    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_SONG_KEY_URL, false).arg(file).arg(mid));
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(REFER_URL, false).toUtf8());
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(QQ_UA_URL, ALG_UA_KEY, false).toUtf8());
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

    QJson::Parser parser;
    bool ok;
    const QVariant &data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("code") && value["code"].toInt() == 0)
        {
            QString url;
            if(value.contains("req_0"))
            {
                QVariantMap req_0 = value["req_0"].toMap();
                req_0 = req_0["data"].toMap();
                const QVariantList &datas = req_0["midurlinfo"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    req_0 = var.toMap();
                    if(req_0.contains("purl"))
                    {
                        url = req_0["purl"].toString();
                        break;
                    }
                }
            }

            if(value.contains("req") && file.startsWith("M500"))
            {
                QVariantMap req = value["req"].toMap();
                req = req["data"].toMap();
                if(req.contains("keepalivefile"))
                {
                    url = req["keepalivefile"].toString();
                }
            }

            return url.isEmpty() ? url : MusicUtils::Algorithm::mdII(QQ_SONG_PREFIX_URL, false) + url;
        }
    }

    return QString();
}
