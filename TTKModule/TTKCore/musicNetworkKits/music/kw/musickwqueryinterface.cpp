#include "musickwqueryinterface.h"
#include "musicabstractnetwork.h"

#include "qalgorithm/deswrapper.h"

void MusicKWInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    TTK::setSslConfiguration(request);
    TTK::makeUserAgentHeader(request);
    TTK::makeContentTypeHeader(request);
}

void MusicKWInterface::parseFromSongProperty(TTK::MusicSongInformation *info, const QString &bitrate, QString &url)
{
    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_SONG_DETAIL_URL, false).arg(info->m_songId).arg(bitrate));
    MusicKWInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    QJson::Parser json;
    bool ok = false;
    const QVariant &data = json.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value["code"].toInt() == 200 && value.contains("data"))
        {
            value = value["data"].toMap();
            if(value.isEmpty())
            {
                return;
            }

            url = value["url"].toString();
        }
    }
}

void MusicKWInterface::makeCoverPixmapUrl(QString &url)
{
    if(!TTK::String::isNetworkUrl(url))
    {
        url = TTK::Algorithm::mdII(KW_ALBUM_COVER_URL, false) + url.section('/', 1);
    }
}

void MusicKWInterface::parseFromSongProperty(TTK::MusicSongInformation *info, const QString &suffix, const QString &format, int bitrate)
{
    if((format.contains("MP3128") || format.contains("128kmp3")) && bitrate == TTK_BN_128 && suffix == MP3_FILE_SUFFIX)
    {
        TTK::MusicSongProperty prop;
        prop.m_bitrate = bitrate;
        prop.m_format = suffix;
        prop.m_size = TTK_DEFAULT_STR;
        parseFromSongProperty(info, "128kmp3", prop.m_url);
        info->m_songProps.append(prop);
    }
    else if((format.contains("MP3192") || format.contains("192kmp3")) && bitrate == TTK_BN_192 && suffix == MP3_FILE_SUFFIX)
    {
        TTK::MusicSongProperty prop;
        prop.m_bitrate = bitrate;
        prop.m_format = suffix;
        prop.m_size = TTK_DEFAULT_STR;
        parseFromSongProperty(info, "192kmp3", prop.m_url);
        info->m_songProps.append(prop);
    }
    else if((format.contains("MP3H") || format.contains("320kmp3")) && bitrate == TTK_BN_320 && suffix == MP3_FILE_SUFFIX)
    {
        TTK::MusicSongProperty prop;
        prop.m_bitrate = bitrate;
        prop.m_format = suffix;
        prop.m_size = TTK_DEFAULT_STR;
        parseFromSongProperty(info, "320kmp3", prop.m_url);
        info->m_songProps.append(prop);
    }
}

void MusicKWInterface::parseFromSongProperty(TTK::MusicSongInformation *info, const QString &format, TTK::QueryQuality quality, bool all)
{
    if(all)
    {
        parseFromSongProperty(info, MP3_FILE_SUFFIX, format, TTK_BN_128);
//        parseFromSongProperty(info, MP3_FILE_SUFFIX, format, TTK_BN_192);
//        parseFromSongProperty(info, MP3_FILE_SUFFIX, format, TTK_BN_320);
//        parseFromSongProperty(info, APE_FILE_SUFFIX, format, TTK_BN_750);
//        parseFromSongProperty(info, FLAC_FILE_SUFFIX, format, TTK_BN_1000);
    }
    else
    {
        if(quality == TTK::QueryQuality::Standard)
        {
            parseFromSongProperty(info, MP3_FILE_SUFFIX, format, TTK_BN_128);
        }
//        else if(quality == TTK::QueryQuality::High)
//        {
//            parseFromSongProperty(info, MP3_FILE_SUFFIX, format, TTK_BN_192);
//        }
//        else if(quality == TTK::QueryQuality::Super)
//        {
//            parseFromSongProperty(info, MP3_FILE_SUFFIX, format, TTK_BN_320);
//        }
//        else if(quality == TTK::QueryQuality::Lossless)
//        {
//            parseFromSongProperty(info, APE_FILE_SUFFIX, format, TTK_BN_750);
//            parseFromSongProperty(info, FLAC_FILE_SUFFIX, format, TTK_BN_1000);
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
