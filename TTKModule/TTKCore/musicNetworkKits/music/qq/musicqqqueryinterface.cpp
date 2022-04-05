#include "musicqqqueryinterface.h"
#include "musicabstractnetwork.h"

#define REFER_URL   "M25YVkpIeHVOaVFRY0k3dmFWOFJsOE1tU013ZWV0Sy8="

void MusicQQInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    request->setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(QQ_UA_URL, ALG_UA_KEY, false).toUtf8());
    request->setRawHeader("Cookie", MusicUtils::Algorithm::mdII(QQ_COOKIE_URL, false).toUtf8());
    MusicObject::setSslConfiguration(request);
}


void MusicQQQueryInterface::readFromMusicSongProperty(MusicObject::MusicSongInformation *info, const QVariantMap &key, int bitrate) const
{
    const QString &mid = key["songmid"].toString();
    if(key["size128"].toULongLong() != 0 && bitrate == MB_128)
    {
        const QString &musicUrl = generateMusicPath("M500" + mid + MP3_FILE, mid);
        if(musicUrl.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongProperty prop;
        prop.m_url = musicUrl;
        prop.m_size = MusicUtils::Number::sizeByte2Label(key["size128"].toULongLong());
        prop.m_format = MP3_FILE_PREFIX;
        prop.m_bitrate = bitrate;
        info->m_songProps.append(prop);
    }
    else if(key["size320"].toULongLong() != 0 && bitrate == MB_320)
    {
        const QString &musicUrl = generateMusicPath("M800" + mid + MP3_FILE, mid);
        if(musicUrl.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongProperty prop;
        prop.m_url = musicUrl;
        prop.m_size = MusicUtils::Number::sizeByte2Label(key["size320"].toULongLong());
        prop.m_format = MP3_FILE_PREFIX;
        prop.m_bitrate = bitrate;
        info->m_songProps.append(prop);
    }
    else if(key["sizeape"].toULongLong() != 0 && bitrate == MB_750)
    {
        const QString &musicUrl = generateMusicPath("A000" + mid + APE_FILE, mid);
        if(musicUrl.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongProperty prop;
        prop.m_url = musicUrl;
        prop.m_size = MusicUtils::Number::sizeByte2Label(key["sizeape"].toULongLong());
        prop.m_format = APE_FILE_PREFIX;
        prop.m_bitrate = bitrate;
        info->m_songProps.append(prop);
    }
    else if(key["sizeflac"].toULongLong() != 0 && bitrate == MB_1000)
    {
        const QString &musicUrl = generateMusicPath("F000" + mid + FLAC_FILE, mid);
        if(musicUrl.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongProperty prop;
        prop.m_url = musicUrl;
        prop.m_size = MusicUtils::Number::sizeByte2Label(key["sizeflac"].toULongLong());
        prop.m_format = FLAC_FILE_PREFIX;
        prop.m_bitrate = bitrate;
        info->m_songProps.append(prop);
    }
}

void MusicQQQueryInterface::readFromMusicSongProperty(MusicObject::MusicSongInformation *info, const QVariantMap &key, const QString &quality, bool all) const
{
    if(all)
    {
        readFromMusicSongProperty(info, key, MB_128);
        readFromMusicSongProperty(info, key, MB_192);
        readFromMusicSongProperty(info, key, MB_320);
        readFromMusicSongProperty(info, key, MB_750);
        readFromMusicSongProperty(info, key, MB_1000);
    }
    else
    {
        if(quality == QObject::tr("SD"))
        {
            readFromMusicSongProperty(info, key, MB_128);
        }
        else if(quality == QObject::tr("HQ"))
        {
            readFromMusicSongProperty(info, key, MB_192);
        }
        else if(quality == QObject::tr("SQ"))
        {
            readFromMusicSongProperty(info, key, MB_320);
        }
        else if(quality == QObject::tr("CD"))
        {
            readFromMusicSongProperty(info, key, MB_750);
            readFromMusicSongProperty(info, key, MB_1000);
        }
    }
}

void MusicQQQueryInterface::readFromMusicSongPropertyNew(MusicObject::MusicSongInformation *info, const QVariantMap &key, int bitrate) const
{
    const QString &mid = key["media_mid"].toString();
    if(key["size_128mp3"].toULongLong() != 0 && bitrate == MB_128)
    {
        const QString &musicUrl = generateMusicPath("M500" + mid + MP3_FILE, mid);
        if(musicUrl.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongProperty prop;
        prop.m_url = musicUrl;
        prop.m_size = MusicUtils::Number::sizeByte2Label(key["size_128mp3"].toULongLong());
        prop.m_format = MP3_FILE_PREFIX;
        prop.m_bitrate = bitrate;
        info->m_songProps.append(prop);
    }
    else if(key["size_320mp3"].toULongLong() != 0 && bitrate == MB_320)
    {
        const QString &musicUrl = generateMusicPath("M800" + mid + MP3_FILE, mid);
        if(musicUrl.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongProperty prop;
        prop.m_url = musicUrl;
        prop.m_size = MusicUtils::Number::sizeByte2Label(key["size_320mp3"].toULongLong());
        prop.m_format = MP3_FILE_PREFIX;
        prop.m_bitrate = bitrate;
        info->m_songProps.append(prop);
    }
    else if(key["size_ape"].toULongLong() != 0 && bitrate == MB_750)
    {
        const QString &musicUrl = generateMusicPath("A000" + mid + APE_FILE, mid);
        if(musicUrl.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongProperty prop;
        prop.m_url = musicUrl;
        prop.m_size = MusicUtils::Number::sizeByte2Label(key["size_ape"].toULongLong());
        prop.m_format = APE_FILE_PREFIX;
        prop.m_bitrate = bitrate;
        info->m_songProps.append(prop);
    }
    else if(key["size_flac"].toULongLong() != 0 && bitrate == MB_1000)
    {
        const QString &musicUrl = generateMusicPath("F000" + mid + FLAC_FILE, mid);
        if(musicUrl.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongProperty prop;
        prop.m_url = musicUrl;
        prop.m_size = MusicUtils::Number::sizeByte2Label(key["size_flac"].toULongLong());
        prop.m_format = FLAC_FILE_PREFIX;
        prop.m_bitrate = bitrate;
        info->m_songProps.append(prop);
    }
}

void MusicQQQueryInterface::readFromMusicSongPropertyNew(MusicObject::MusicSongInformation *info, const QVariantMap &key) const
{
    readFromMusicSongPropertyNew(info, key, MB_128);
    readFromMusicSongPropertyNew(info, key, MB_192);
    readFromMusicSongPropertyNew(info, key, MB_320);
    readFromMusicSongPropertyNew(info, key, MB_750);
    readFromMusicSongPropertyNew(info, key, MB_1000);
}

QString MusicQQQueryInterface::generateMusicPath(const QString &file, const QString &mid) const
{
    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_SONG_KEY_URL, false).arg(file, mid));
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(REFER_URL, false).toUtf8());
    MusicQQInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = MusicObject::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return QString();
    }

    QJson::Parser json;
    bool ok;
    const QVariant &data = json.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("code") && value["code"].toInt() == 0)
        {
            QString url;
            if(value.contains("req"))
            {
                QVariantMap req = value["req"].toMap();
                req = req["data"].toMap();

                QString url_prefix;
                if(req.contains("sip"))
                {
                    const QVariantList &sip = req["sip"].toList();
                    if(sip.count() > 0)
                    {
                        url_prefix = sip[0].toString();
                    }
                }

                const QVariantList &info = req["midurlinfo"].toList();
                for(const QVariant &var : qAsConst(info))
                {
                    req = var.toMap();
                    if(req.contains("purl"))
                    {
                        url = req["purl"].toString();
                        break;
                    }
                }

                if(!url_prefix.isEmpty() && !url.isEmpty())
                {
                    url = url_prefix + url;
                    url.replace("\u0026", "&");
                }
            }

            return url;
        }
    }

    return QString();
}
