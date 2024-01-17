#include "musickwqueryinterface.h"
#include "musicabstractnetwork.h"

#include "qalgorithm/deswrapper.h"

void MusicKWInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    TTK::setSslConfiguration(request);
    TTK::makeUserAgentHeader(request);
    TTK::makeContentTypeHeader(request);
}

QString MusicKWInterface::makeCoverPixmapUrl(const QString &url, const QString &id)
{
    if(url.isEmpty() && !id.isEmpty())
    {
        return TTK::Algorithm::mdII(KW_ALBUM_COVER_INFO_URL, false).arg(id);
    }
    else if(!TTK::String::isNetworkUrl(url))
    {
        return TTK::Algorithm::mdII(KW_ALBUM_COVER_URL, false) + url.section('/', 1);
    }
    else
    {
        return url;
    }
}

static void parseSongProperty(TTK::MusicSongInformation *info)
{
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

static void parseSongProperty(TTK::MusicSongInformation *info, const QString &suffix, const QString &bitrate, QString &url)
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

static void parseSongProperty(TTK::MusicSongInformation *info, const QString &suffix, const QString &format, int bitrate)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == bitrate)
        {
            return;
        }
    }

    TTK::MusicSongProperty prop;
    prop.m_bitrate = bitrate;
    prop.m_format = suffix;
    prop.m_size = TTK_DEFAULT_STR;

    if((format.contains("MP3128") || format.contains("128kmp3")) && bitrate == TTK_BN_128)
    {
        parseSongProperty(info, suffix, "128kmp3", prop.m_url);
    }
    else if((format.contains("MP3192") || format.contains("192kmp3")) && bitrate == TTK_BN_192)
    {
        parseSongProperty(info, suffix, "192kmp3", prop.m_url);
    }
    else if((format.contains("MP3H") || format.contains("320kmp3")) && bitrate == TTK_BN_320 )
    {
        parseSongProperty(info, suffix, "320kmp3", prop.m_url);
    }
    else if((format.contains("FLAC") || format.contains("2000kflac")) && bitrate == TTK_BN_1000)
    {
        parseSongProperty(info, suffix, "2000kflac", prop.m_url);
    }

    if(!prop.isEmpty())
    {
        info->m_songProps.append(prop);
    }
}

void MusicKWInterface::parseFromSongProperty(TTK::MusicSongInformation *info, int bitrate)
{
    if(info->m_formatProps.isEmpty())
    {
        parseSongProperty(info); //find 128kmp3 first
        parseSongProperty(info, MP3_FILE_SUFFIX, "128kmp3", TTK_BN_128);
        return;
    }

    if(bitrate == TTK_BN_0)
    {
        parseSongProperty(info); //find 128kmp3 first
        parseSongProperty(info, MP3_FILE_SUFFIX, info->m_formatProps, TTK_BN_128);
        parseSongProperty(info, MP3_FILE_SUFFIX, info->m_formatProps, TTK_BN_192);
        parseSongProperty(info, MP3_FILE_SUFFIX, info->m_formatProps, TTK_BN_320);
        parseSongProperty(info, FLAC_FILE_SUFFIX, info->m_formatProps, TTK_BN_1000);
    }
    else
    {
        if(bitrate == TTK_BN_128)
        {
            parseSongProperty(info); //find 128kmp3 first
        }
        parseSongProperty(info, bitrate > TTK_BN_320 ? FLAC_FILE_SUFFIX : MP3_FILE_SUFFIX, info->m_formatProps, bitrate);
    }
}

void MusicKWInterface::parseFromSongProperty(TTK::MusicSongInformation *info, const QString &format)
{
    info->m_formatProps = format;
}
