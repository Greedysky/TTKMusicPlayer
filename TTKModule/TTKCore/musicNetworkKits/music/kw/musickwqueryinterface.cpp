#include "musickwqueryinterface.h"
#include "musicabstractnetwork.h"

#include "qalgorithm/deswrapper.h"

void MusicKWInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    request->setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KW_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(request);
}


void MusicKWQueryInterface::readFromMusicHighProperty(MusicObject::MusicSongInformation *info, const QString &suffix, const QString &format, int bitrate) const
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    QAlgorithm::Des des;
    const QByteArray &parameter = des.encrypt(MusicUtils::Algorithm::mdII(KW_SONG_DETAIL_DATA_URL, false).arg(info->m_songId, suffix, format).toUtf8(),
                                              MusicUtils::Algorithm::mdII(_SIGN, ALG_UNIMP_KEY, false).toUtf8());
    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KW_MOVIE_URL, false).arg(parameter.constData()));
    MusicKWInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = MusicObject::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    if(!bytes.isEmpty() && !bytes.contains("res not found"))
    {
        const QString text(bytes);
        QRegExp regx(".*url=(.*)\r\nsig=");

        if(text.indexOf(regx) != -1)
        {
            MusicObject::MusicSongProperty prop;
            prop.m_url = regx.cap(1);
            prop.m_bitrate = bitrate;
            prop.m_format = suffix;
            prop.m_size = TTK_DEFAULT_STR;
            if(prop.m_url.isEmpty() || info->m_songProps.contains(prop))
            {
                return;
            }

            info->m_songProps.append(prop);
        }
    }
}

void MusicKWQueryInterface::readFromMusicSongProperty(MusicObject::MusicSongInformation *info, const QString &suffix, const QString &format, int bitrate) const
{
    if(format.contains("MP3128") && bitrate == MB_128 && suffix == MP3_FILE_PREFIX)
    {
        MusicObject::MusicSongProperty prop;
        prop.m_bitrate = bitrate;
        prop.m_format = suffix;
        prop.m_size = TTK_DEFAULT_STR;
        prop.m_url = MusicUtils::Algorithm::mdII(KW_SONG_DETAIL_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songProps.append(prop);
    }
    else if(format.contains("MP3192") && bitrate == MB_192 && suffix == MP3_FILE_PREFIX)
    {
        MusicObject::MusicSongProperty prop;
        prop.m_bitrate = bitrate;
        prop.m_format = suffix;
        prop.m_size = TTK_DEFAULT_STR;
        prop.m_url = MusicUtils::Algorithm::mdII(KW_SONG_DETAIL_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songProps.append(prop);
    }
    else if(format.contains("MP3H") && bitrate == MB_320 && suffix == MP3_FILE_PREFIX)
    {
        MusicObject::MusicSongProperty prop;
        prop.m_bitrate = bitrate;
        prop.m_format = suffix;
        prop.m_size = TTK_DEFAULT_STR;
        prop.m_url = MusicUtils::Algorithm::mdII(KW_SONG_DETAIL_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songProps.append(prop);
    }
    else if(format.contains("ALFLAC") && bitrate == MB_1000 && suffix == FLAC_FILE_PREFIX)
    {
        readFromMusicHighProperty(info, FLAC_FILE_PREFIX, "2000kflac", bitrate);
    }
    else if(format.contains("AL") && bitrate == MB_750 && suffix == APE_FILE_PREFIX)
    {
        readFromMusicHighProperty(info, APE_FILE_PREFIX, "1000kape", bitrate);
    }
}

void MusicKWQueryInterface::readFromMusicSongPropertyNew(MusicObject::MusicSongInformation *info, const QString &suffix, const QString &format, int bitrate) const
{
    if(format.contains("128kmp3") && bitrate == MB_128 && suffix == MP3_FILE_PREFIX)
    {
        MusicObject::MusicSongProperty prop;
        prop.m_bitrate = bitrate;
        prop.m_format = suffix;
        prop.m_size = TTK_DEFAULT_STR;
        prop.m_url = MusicUtils::Algorithm::mdII(KW_SONG_DETAIL_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songProps.append(prop);
    }
    else if(format.contains("192kmp3") && bitrate == MB_192 && suffix == MP3_FILE_PREFIX)
    {
        MusicObject::MusicSongProperty prop;
        prop.m_bitrate = bitrate;
        prop.m_format = suffix;
        prop.m_size = TTK_DEFAULT_STR;
        prop.m_url = MusicUtils::Algorithm::mdII(KW_SONG_DETAIL_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songProps.append(prop);
    }
    else if(format.contains("320kmp3") && bitrate == MB_320 && suffix == MP3_FILE_PREFIX)
    {
        MusicObject::MusicSongProperty prop;
        prop.m_bitrate = bitrate;
        prop.m_format = suffix;
        prop.m_size = TTK_DEFAULT_STR;
        prop.m_url = MusicUtils::Algorithm::mdII(KW_SONG_DETAIL_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songProps.append(prop);
    }
    else if(format.contains("2000kflac") && bitrate == MB_1000 && suffix == FLAC_FILE_PREFIX)
    {
        readFromMusicHighProperty(info, FLAC_FILE_PREFIX, "2000kflac", bitrate);
    }
    else if(format.contains("1000kape") && bitrate == MB_750 && suffix == APE_FILE_PREFIX)
    {
        readFromMusicHighProperty(info, APE_FILE_PREFIX, "1000kape", bitrate);
    }
}

void MusicKWQueryInterface::readFromMusicSongProperty(MusicObject::MusicSongInformation *info, const QString &format, MusicObject::QueryQuality quality, bool all) const
{
    if(all)
    {
        readFromMusicSongProperty(info, MP3_FILE_PREFIX, format, MB_128);
        readFromMusicSongProperty(info, MP3_FILE_PREFIX, format, MB_192);
        readFromMusicSongProperty(info, MP3_FILE_PREFIX, format, MB_320);
        readFromMusicSongProperty(info, APE_FILE_PREFIX, format, MB_750);
        readFromMusicSongProperty(info, FLAC_FILE_PREFIX, format, MB_1000);
    }
    else
    {
        if(quality == MusicObject::StandardQuality)
        {
            readFromMusicSongProperty(info, MP3_FILE_PREFIX, format, MB_128);
        }
        else if(quality == MusicObject::HighQuality)
        {
            readFromMusicSongProperty(info, MP3_FILE_PREFIX, format, MB_192);
        }
        else if(quality == MusicObject::SuperQuality)
        {
            readFromMusicSongProperty(info, MP3_FILE_PREFIX, format, MB_320);
        }
        else if(quality == MusicObject::LosslessQuality)
        {
            readFromMusicSongProperty(info, APE_FILE_PREFIX, format, MB_750);
            readFromMusicSongProperty(info, FLAC_FILE_PREFIX, format, MB_1000);
        }
    }
}

void MusicKWQueryInterface::readFromMusicSongProperty(MusicObject::MusicSongInformation *info, const QVariantList &format, MusicObject::QueryQuality quality, bool all) const
{
    for(const QVariant &var : qAsConst(format))
    {
        const QString &fs = var.toString();
        if(all)
        {
            readFromMusicSongPropertyNew(info, MP3_FILE_PREFIX, fs, MB_128);
            readFromMusicSongPropertyNew(info, MP3_FILE_PREFIX, fs, MB_192);
            readFromMusicSongPropertyNew(info, MP3_FILE_PREFIX, fs, MB_320);
            readFromMusicSongPropertyNew(info, APE_FILE_PREFIX, fs, MB_750);
            readFromMusicSongPropertyNew(info, FLAC_FILE_PREFIX, fs, MB_1000);
        }
        else
        {
            if(quality == MusicObject::StandardQuality)
            {
                readFromMusicSongPropertyNew(info, MP3_FILE_PREFIX, fs, MB_128);
            }
            else if(quality == MusicObject::HighQuality)
            {
                readFromMusicSongPropertyNew(info, MP3_FILE_PREFIX, fs, MB_192);
            }
            else if(quality == MusicObject::SuperQuality)
            {
                readFromMusicSongPropertyNew(info, MP3_FILE_PREFIX, fs, MB_320);
            }
            else if(quality == MusicObject::LosslessQuality)
            {
                readFromMusicSongPropertyNew(info, APE_FILE_PREFIX, fs, MB_750);
                readFromMusicSongPropertyNew(info, FLAC_FILE_PREFIX, fs, MB_1000);
            }
        }
    }
}

void MusicKWQueryInterface::readFromMusicSongPicture(MusicObject::MusicSongInformation *info) const
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KW_SONG_LRC_URL, false).arg(info->m_songId));
    MusicKWInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = MusicObject::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    QJson::Parser json;
    bool ok;
    const QVariant &data = json.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("data"))
        {
            value = value["data"].toMap();
            value = value["songinfo"].toMap();
            info->m_coverUrl = value["pic"].toString();
            if(!info->m_coverUrl.contains(HTTP_PREFIX) && !info->m_coverUrl.contains(TTK_NULL_STR))
            {
                info->m_coverUrl = MusicUtils::Algorithm::mdII(KW_ALBUM_COVER_URL, false) + info->m_coverUrl;
            }

            if(info->m_duration.isEmpty())
            {
                info->m_duration = MusicTime::msecTime2LabelJustified(value["duration"].toInt() * 1000);
            }
        }
    }
}
