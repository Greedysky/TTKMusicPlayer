#include "musicbdqueryinterface.h"
#include "musicabstractqueryrequest.h"

void MusicBDInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    request->setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(request);
}


void MusicBDQueryInterface::parseFromSongProperty(MusicObject::MusicSongInformation *info, const QString &key, int length, int bitrate) const
{
    if(key.isEmpty())
    {
        return;
    }

    MusicObject::MusicSongProperty prop;
    prop.m_url = MusicUtils::Algorithm::mdII(BD_BASE_URL, false) + key;
    prop.m_size = MusicUtils::Number::sizeByte2Label(length * 1000 * bitrate / 8);
    switch(bitrate)
    {
    case MB_128: prop.m_format = MP3_FILE_PREFIX; break;
    case MB_192: prop.m_format = MP3_FILE_PREFIX; break;
    case MB_320: prop.m_format = MP3_FILE_PREFIX; break;
    case MB_750: prop.m_format = APE_FILE_PREFIX; break;
    case MB_1000: prop.m_format = FLAC_FILE_PREFIX; break;
    default: prop.m_format = MP3_FILE_PREFIX; break;
    }
    prop.m_bitrate = bitrate;
    info->m_songProps.append(prop);
}

void MusicBDQueryInterface::parseFromSongProperty(MusicObject::MusicSongInformation *info, const QVariantMap &key, MusicObject::QueryQuality quality, bool all) const
{
    info->m_lrcUrl = MusicUtils::Algorithm::mdII(BD_BASE_URL, false) + key["lrcUrl"].toString();
    info->m_coverUrl = key["picUrl"].toString();
    const int length = key["length"].toInt();

    if(all)
    {
        parseFromSongProperty(info, key["lqUrl"].toString(), length, MB_128);
        parseFromSongProperty(info, key["hqUrl"].toString(), length, MB_192);
        parseFromSongProperty(info, key["sqUrl"].toString(), length, MB_320);
        parseFromSongProperty(info, key["apeUrl"].toString(), length, MB_750);
        parseFromSongProperty(info, key["flacUrl"].toString(), length, MB_1000);
    }
    else
    {
        if(quality == MusicObject::QueryQuality::Standard)
        {
            parseFromSongProperty(info, key["lqUrl"].toString(), length, MB_128);
        }
        else if(quality == MusicObject::QueryQuality::High)
        {
            parseFromSongProperty(info, key["hqUrl"].toString(), length, MB_192);
        }
        else if(quality == MusicObject::QueryQuality::Super)
        {
            parseFromSongProperty(info, key["sqUrl"].toString(), length, MB_320);
        }
        else if(quality == MusicObject::QueryQuality::Lossless)
        {
            parseFromSongProperty(info, key["apeUrl"].toString(), length, MB_750);
            parseFromSongProperty(info, key["flacUrl"].toString(), length, MB_1000);
        }
    }

    if(info->m_songProps.isEmpty())
    {
        parseFromSongProperty(info, key["copyUrl"].toString(), length, MB_128);
    }
}
