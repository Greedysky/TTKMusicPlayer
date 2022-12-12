#include "musickwqueryinterface.h"
#include "musicabstractnetwork.h"

#include "qalgorithm/deswrapper.h"

void MusicKWInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    request->setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KW_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(request);
}


void MusicKWQueryInterface::parseFromSongHighProperty(MusicObject::MusicSongInformation *info, const QString &suffix, const QString &format, int bitrate) const
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
        const QRegExp regx(".*url=(.*)\r\nsig=");

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

void MusicKWQueryInterface::parseFromSongProperty(MusicObject::MusicSongInformation *info, const QString &suffix, const QString &format, int bitrate) const
{
    if(format.contains("MP3128") && bitrate == MB_128 && suffix == MP3_FILE_SUFFIX)
    {
        MusicObject::MusicSongProperty prop;
        prop.m_bitrate = bitrate;
        prop.m_format = suffix;
        prop.m_size = TTK_DEFAULT_STR;
        prop.m_url = MusicUtils::Algorithm::mdII(KW_SONG_DETAIL_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songProps.append(prop);
    }
    else if(format.contains("MP3192") && bitrate == MB_192 && suffix == MP3_FILE_SUFFIX)
    {
        MusicObject::MusicSongProperty prop;
        prop.m_bitrate = bitrate;
        prop.m_format = suffix;
        prop.m_size = TTK_DEFAULT_STR;
        prop.m_url = MusicUtils::Algorithm::mdII(KW_SONG_DETAIL_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songProps.append(prop);
    }
    else if(format.contains("MP3H") && bitrate == MB_320 && suffix == MP3_FILE_SUFFIX)
    {
        MusicObject::MusicSongProperty prop;
        prop.m_bitrate = bitrate;
        prop.m_format = suffix;
        prop.m_size = TTK_DEFAULT_STR;
        prop.m_url = MusicUtils::Algorithm::mdII(KW_SONG_DETAIL_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songProps.append(prop);
    }
    else if(format.contains("ALFLAC") && bitrate == MB_1000 && suffix == FLAC_FILE_SUFFIX)
    {
        parseFromSongHighProperty(info, FLAC_FILE_SUFFIX, "2000kflac", bitrate);
    }
    else if(format.contains("AL") && bitrate == MB_750 && suffix == APE_FILE_SUFFIX)
    {
        parseFromSongHighProperty(info, APE_FILE_SUFFIX, "1000kape", bitrate);
    }
}

void MusicKWQueryInterface::parseFromSongPropertyNew(MusicObject::MusicSongInformation *info, const QString &suffix, const QString &format, int bitrate) const
{
    if(format.contains("128kmp3") && bitrate == MB_128 && suffix == MP3_FILE_SUFFIX)
    {
        MusicObject::MusicSongProperty prop;
        prop.m_bitrate = bitrate;
        prop.m_format = suffix;
        prop.m_size = TTK_DEFAULT_STR;
        prop.m_url = MusicUtils::Algorithm::mdII(KW_SONG_DETAIL_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songProps.append(prop);
    }
    else if(format.contains("192kmp3") && bitrate == MB_192 && suffix == MP3_FILE_SUFFIX)
    {
        MusicObject::MusicSongProperty prop;
        prop.m_bitrate = bitrate;
        prop.m_format = suffix;
        prop.m_size = TTK_DEFAULT_STR;
        prop.m_url = MusicUtils::Algorithm::mdII(KW_SONG_DETAIL_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songProps.append(prop);
    }
    else if(format.contains("320kmp3") && bitrate == MB_320 && suffix == MP3_FILE_SUFFIX)
    {
        MusicObject::MusicSongProperty prop;
        prop.m_bitrate = bitrate;
        prop.m_format = suffix;
        prop.m_size = TTK_DEFAULT_STR;
        prop.m_url = MusicUtils::Algorithm::mdII(KW_SONG_DETAIL_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songProps.append(prop);
    }
    else if(format.contains("2000kflac") && bitrate == MB_1000 && suffix == FLAC_FILE_SUFFIX)
    {
        parseFromSongHighProperty(info, FLAC_FILE_SUFFIX, "2000kflac", bitrate);
    }
    else if(format.contains("1000kape") && bitrate == MB_750 && suffix == APE_FILE_SUFFIX)
    {
        parseFromSongHighProperty(info, APE_FILE_SUFFIX, "1000kape", bitrate);
    }
}

void MusicKWQueryInterface::parseFromSongProperty(MusicObject::MusicSongInformation *info, const QString &format, MusicObject::QueryQuality quality, bool all) const
{
    if(all)
    {
        parseFromSongProperty(info, MP3_FILE_SUFFIX, format, MB_128);
        parseFromSongProperty(info, MP3_FILE_SUFFIX, format, MB_192);
        parseFromSongProperty(info, MP3_FILE_SUFFIX, format, MB_320);
        parseFromSongProperty(info, APE_FILE_SUFFIX, format, MB_750);
        parseFromSongProperty(info, FLAC_FILE_SUFFIX, format, MB_1000);
    }
    else
    {
        if(quality == MusicObject::QueryQuality::Standard)
        {
            parseFromSongProperty(info, MP3_FILE_SUFFIX, format, MB_128);
        }
        else if(quality == MusicObject::QueryQuality::High)
        {
            parseFromSongProperty(info, MP3_FILE_SUFFIX, format, MB_192);
        }
        else if(quality == MusicObject::QueryQuality::Super)
        {
            parseFromSongProperty(info, MP3_FILE_SUFFIX, format, MB_320);
        }
        else if(quality == MusicObject::QueryQuality::Lossless)
        {
            parseFromSongProperty(info, APE_FILE_SUFFIX, format, MB_750);
            parseFromSongProperty(info, FLAC_FILE_SUFFIX, format, MB_1000);
        }
    }
}

void MusicKWQueryInterface::parseFromSongProperty(MusicObject::MusicSongInformation *info, const QVariantList &format, MusicObject::QueryQuality quality, bool all) const
{
    for(const QVariant &var : qAsConst(format))
    {
        const QString &fs = var.toString();
        if(all)
        {
            parseFromSongPropertyNew(info, MP3_FILE_SUFFIX, fs, MB_128);
            parseFromSongPropertyNew(info, MP3_FILE_SUFFIX, fs, MB_192);
            parseFromSongPropertyNew(info, MP3_FILE_SUFFIX, fs, MB_320);
            parseFromSongPropertyNew(info, APE_FILE_SUFFIX, fs, MB_750);
            parseFromSongPropertyNew(info, FLAC_FILE_SUFFIX, fs, MB_1000);
        }
        else
        {
            if(quality == MusicObject::QueryQuality::Standard)
            {
                parseFromSongPropertyNew(info, MP3_FILE_SUFFIX, fs, MB_128);
            }
            else if(quality == MusicObject::QueryQuality::High)
            {
                parseFromSongPropertyNew(info, MP3_FILE_SUFFIX, fs, MB_192);
            }
            else if(quality == MusicObject::QueryQuality::Super)
            {
                parseFromSongPropertyNew(info, MP3_FILE_SUFFIX, fs, MB_320);
            }
            else if(quality == MusicObject::QueryQuality::Lossless)
            {
                parseFromSongPropertyNew(info, APE_FILE_SUFFIX, fs, MB_750);
                parseFromSongPropertyNew(info, FLAC_FILE_SUFFIX, fs, MB_1000);
            }
        }
    }
}
