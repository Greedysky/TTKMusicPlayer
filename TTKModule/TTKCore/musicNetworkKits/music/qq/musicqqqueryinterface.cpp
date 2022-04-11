#include "musicqqqueryinterface.h"
#include "musicabstractnetwork.h"

#define REFER_URL   "M25YVkpIeHVOaVFRY0k3dmFWOFJsOE1tU013ZWV0Sy8="

void MusicQQInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    request->setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(QQ_UA_URL, ALG_UA_KEY, false).toUtf8());
    request->setRawHeader("Cookie", MusicUtils::Algorithm::mdII(QQ_COOKIE_URL, false).toUtf8());
    MusicObject::setSslConfiguration(request);
}


void MusicQQQueryInterface::readFromMusicSongProperty(MusicObject::MusicSongInformation *info, const QString &mid, quint64 key, int bitrate) const
{
    if(key == 0)
    {
        return;
    }

    if(bitrate == MB_128)
    {
        const QString &musicUrl = generateMusicPath("M500" + mid + MP3_FILE, mid);
        if(musicUrl.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongProperty prop;
        prop.m_url = musicUrl;
        prop.m_size = MusicUtils::Number::sizeByte2Label(key);
        prop.m_format = MP3_FILE_PREFIX;
        prop.m_bitrate = bitrate;
        info->m_songProps.append(prop);
    }
    else if(bitrate == MB_320)
    {
        const QString &musicUrl = generateMusicPath("M800" + mid + MP3_FILE, mid);
        if(musicUrl.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongProperty prop;
        prop.m_url = musicUrl;
        prop.m_size = MusicUtils::Number::sizeByte2Label(key);
        prop.m_format = MP3_FILE_PREFIX;
        prop.m_bitrate = bitrate;
        info->m_songProps.append(prop);
    }
    else if(bitrate == MB_750)
    {
        const QString &musicUrl = generateMusicPath("A000" + mid + APE_FILE, mid);
        if(musicUrl.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongProperty prop;
        prop.m_url = musicUrl;
        prop.m_size = MusicUtils::Number::sizeByte2Label(key);
        prop.m_format = APE_FILE_PREFIX;
        prop.m_bitrate = bitrate;
        info->m_songProps.append(prop);
    }
    else if(bitrate == MB_1000)
    {
        const QString &musicUrl = generateMusicPath("F000" + mid + FLAC_FILE, mid);
        if(musicUrl.isEmpty())
        {
            return;
        }

        MusicObject::MusicSongProperty prop;
        prop.m_url = musicUrl;
        prop.m_size = MusicUtils::Number::sizeByte2Label(key);
        prop.m_format = FLAC_FILE_PREFIX;
        prop.m_bitrate = bitrate;
        info->m_songProps.append(prop);
    }
}

void MusicQQQueryInterface::readFromMusicSongProperty(MusicObject::MusicSongInformation *info, const QVariantMap &key, MusicObject::QueryQuality quality, bool all) const
{
    const QString &mid = key["songmid"].toString();

    if(all)
    {
        readFromMusicSongProperty(info, mid, key["size128"].toULongLong(), MB_128);
        readFromMusicSongProperty(info, mid, key["size192"].toULongLong(), MB_192);
        readFromMusicSongProperty(info, mid, key["size320"].toULongLong(), MB_320);
        readFromMusicSongProperty(info, mid, key["sizeape"].toULongLong(), MB_750);
        readFromMusicSongProperty(info, mid, key["sizeflac"].toULongLong(), MB_1000);
    }
    else
    {
        if(quality == MusicObject::StandardQuality)
        {
            readFromMusicSongProperty(info, mid, key["size128"].toULongLong(), MB_128);
        }
        else if(quality == MusicObject::HighQuality)
        {
            readFromMusicSongProperty(info, mid, key["size192"].toULongLong(), MB_192);
        }
        else if(quality == MusicObject::SuperQuality)
        {
            readFromMusicSongProperty(info, mid, key["size320"].toULongLong(), MB_320);
        }
        else if(quality == MusicObject::LosslessQuality)
        {
            readFromMusicSongProperty(info, mid, key["sizeape"].toULongLong(), MB_750);
            readFromMusicSongProperty(info, mid, key["sizeflac"].toULongLong(), MB_1000);
        }
    }
}

void MusicQQQueryInterface::readFromMusicSongPropertyNew(MusicObject::MusicSongInformation *info, const QVariantMap &key) const
{
    const QString &mid = key["media_mid"].toString();

    readFromMusicSongProperty(info, mid, key["size_128mp3"].toULongLong(), MB_128);
    readFromMusicSongProperty(info, mid, key["size_192mp3"].toULongLong(), MB_192);
    readFromMusicSongProperty(info, mid, key["size_320mp3"].toULongLong(), MB_320);
    readFromMusicSongProperty(info, mid, key["size_ape"].toULongLong(), MB_750);
    readFromMusicSongProperty(info, mid, key["size_flac"].toULongLong(), MB_1000);
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

                QString prefix;
                if(req.contains("sip"))
                {
                    const QVariantList &sip = req["sip"].toList();
                    if(sip.count() > 0)
                    {
                        prefix = sip[0].toString();
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

                if(!prefix.isEmpty() && !url.isEmpty())
                {
                    url = prefix + url;
                    url.replace("\u0026", "&");
                }
            }

            return url;
        }
    }

    return QString();
}
