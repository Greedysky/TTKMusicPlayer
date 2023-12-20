#include "musickwqueryinterface.h"
#include "musicabstractnetwork.h"

#include "qalgorithm/deswrapper.h"

void MusicKWInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    TTK::setSslConfiguration(request);
    TTK::makeUserAgentHeader(request);
    TTK::makeContentTypeHeader(request);
}

void MusicKWInterface::parseFromSongProperty(TTK::MusicSongInformation *info)
{
    if(!info->m_songProps.isEmpty())
    {
        return;
    }

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_SONG_DETAIL_CGG_URL, false).arg(info->m_songId));
    MusicKWInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    TTK::MusicSongProperty prop;
    prop.m_bitrate = TTK_BN_128;
    prop.m_format = MP3_FILE_SUFFIX;
    prop.m_size = TTK_DEFAULT_STR;
    prop.m_url = bytes;
    info->m_songProps.append(prop);
}

void MusicKWInterface::parseFromSongProperty(TTK::MusicSongInformation *info, const QString &suffix, const QString &bitrate, QString &url)
{
    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_SONG_DETAIL_URL, false).arg(info->m_songId, bitrate));
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

            const QString &format = value["format"].toString();
            if(format == suffix)
            {
                url = value["url"].toString();
            }
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
    TTK::MusicSongProperty prop;
    prop.m_bitrate = bitrate;
    prop.m_format = suffix;
    prop.m_size = TTK_DEFAULT_STR;

    if((format.contains("MP3128") || format.contains("128kmp3")) && bitrate == TTK_BN_128 && suffix == MP3_FILE_SUFFIX)
    {
        parseFromSongProperty(info, suffix, "128kmp3", prop.m_url);
    }
    else if((format.contains("MP3192") || format.contains("192kmp3")) && bitrate == TTK_BN_192 && suffix == MP3_FILE_SUFFIX)
    {
        parseFromSongProperty(info, suffix, "192kmp3", prop.m_url);
    }
    else if((format.contains("MP3H") || format.contains("320kmp3")) && bitrate == TTK_BN_320 && suffix == MP3_FILE_SUFFIX)
    {
        parseFromSongProperty(info, suffix, "320kmp3", prop.m_url);
    }
    else if((format.contains("FLAC") || format.contains("2000kflac")) && bitrate == TTK_BN_1000 && suffix == FLAC_FILE_SUFFIX)
    {
        parseFromSongProperty(info, suffix, "2000kflac", prop.m_url);
    }

    if(!prop.m_url.isEmpty())
    {
        info->m_songProps.append(prop);
    }
}

void MusicKWInterface::parseFromSongProperty(TTK::MusicSongInformation *info, const QString &format, bool all)
{
    if(all)
    {
        parseFromSongProperty(info, MP3_FILE_SUFFIX, format, TTK_BN_128);
        parseFromSongProperty(info, MP3_FILE_SUFFIX, format, TTK_BN_192);
        parseFromSongProperty(info, MP3_FILE_SUFFIX, format, TTK_BN_320);
        parseFromSongProperty(info, FLAC_FILE_SUFFIX, format, TTK_BN_1000);
    }
    else
    {
        parseFromSongProperty(info, MP3_FILE_SUFFIX, "128kmp3", TTK_BN_128);
    }

    parseFromSongProperty(info);
}

void MusicKWInterface::parseFromSongProperty(TTK::MusicSongInformation *info, const QVariantList &format, bool all)
{
    if(all)
    {
        for(const QVariant &var : qAsConst(format))
        {
            parseFromSongProperty(info, var.toString(), true);
        }
    }
    else
    {
        parseFromSongProperty(info, MP3_FILE_SUFFIX, "128kmp3", TTK_BN_128);
    }

    parseFromSongProperty(info);
}
