#include "musickwqueryinterface.h"
#include "musicabstractnetwork.h"

#include "qalgorithm/deswrapper.h"

void MusicKWInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    request->setRawHeader("User-Agent", TTK::Algorithm::mdII(KW_UA_URL, ALG_UA_KEY, false).toUtf8());
    TTK::setSslConfiguration(request);
}

void MusicKWInterface::parseFromSongProperty(TTK::MusicSongInformation *info, const QString &suffix, const QString &format, int bitrate)
{
    if((format.contains("MP3128") || format.contains("128kmp3")) && bitrate == MB_128 && suffix == MP3_FILE_SUFFIX)
    {
        TTK::MusicSongProperty prop;
        prop.m_bitrate = bitrate;
        prop.m_format = suffix;
        prop.m_size = TTK_DEFAULT_STR;
        prop.m_url = TTK::Algorithm::mdII(KW_SONG_DETAIL_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songProps.append(prop);
    }
    else if((format.contains("MP3192") || format.contains("192kmp3")) && bitrate == MB_192 && suffix == MP3_FILE_SUFFIX)
    {
        TTK::MusicSongProperty prop;
        prop.m_bitrate = bitrate;
        prop.m_format = suffix;
        prop.m_size = TTK_DEFAULT_STR;
        prop.m_url = TTK::Algorithm::mdII(KW_SONG_DETAIL_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songProps.append(prop);
    }
    else if((format.contains("MP3H") || format.contains("320kmp3")) && bitrate == MB_320 && suffix == MP3_FILE_SUFFIX)
    {
        TTK::MusicSongProperty prop;
        prop.m_bitrate = bitrate;
        prop.m_format = suffix;
        prop.m_size = TTK_DEFAULT_STR;
        prop.m_url = TTK::Algorithm::mdII(KW_SONG_DETAIL_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songProps.append(prop);
    }
}

void MusicKWInterface::parseFromSongProperty(TTK::MusicSongInformation *info, const QString &format, TTK::QueryQuality quality, bool all)
{
    if(all)
    {
        parseFromSongProperty(info, MP3_FILE_SUFFIX, format, MB_128);
//        parseFromSongProperty(info, MP3_FILE_SUFFIX, format, MB_192);
//        parseFromSongProperty(info, MP3_FILE_SUFFIX, format, MB_320);
//        parseFromSongProperty(info, APE_FILE_SUFFIX, format, MB_750);
//        parseFromSongProperty(info, FLAC_FILE_SUFFIX, format, MB_1000);
    }
    else
    {
        if(quality == TTK::QueryQuality::Standard)
        {
            parseFromSongProperty(info, MP3_FILE_SUFFIX, format, MB_128);
        }
//        else if(quality == TTK::QueryQuality::High)
//        {
//            parseFromSongProperty(info, MP3_FILE_SUFFIX, format, MB_192);
//        }
//        else if(quality == TTK::QueryQuality::Super)
//        {
//            parseFromSongProperty(info, MP3_FILE_SUFFIX, format, MB_320);
//        }
//        else if(quality == TTK::QueryQuality::Lossless)
//        {
//            parseFromSongProperty(info, APE_FILE_SUFFIX, format, MB_750);
//            parseFromSongProperty(info, FLAC_FILE_SUFFIX, format, MB_1000);
//        }
    }
}

void MusicKWInterface::parseFromSongProperty(TTK::MusicSongInformation *info, const QVariantList &format, TTK::QueryQuality quality, bool all)
{
    for(const QVariant &var : qAsConst(format))
    {
        parseFromSongProperty(info, var.toString(), quality, all);
    }
}
