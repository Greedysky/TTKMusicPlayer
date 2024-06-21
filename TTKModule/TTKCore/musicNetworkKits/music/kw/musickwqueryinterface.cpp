#include "musickwqueryinterface.h"
#include "musicabstractnetwork.h"

void ReqKWInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    TTK::setSslConfiguration(request);
    TTK::makeUserAgentHeader(request);
    TTK::makeContentTypeHeader(request);
}

QString ReqKWInterface::makeSongArtist(const QString &name)
{
    return TTK::String::charactersReplace(name).replace("&", ";").replace("+", ";");
}

QString ReqKWInterface::makeCoverPixmapUrl(const QString &url, const QString &id)
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

static void parseSongPropertyV1(TTK::MusicSongInformation *info, const QString &suffix, const QString &format, int bitrate)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == bitrate)
        {
            return;
        }
    }

    TTK_INFO_STREAM("parse song property in v1 module");

    QString quality;
    if((format.contains("MP3128") || format.contains("128kmp3")) && bitrate == TTK_BN_128)
    {
        quality = "128kmp3";
    }
    else if((format.contains("MP3192") || format.contains("192kmp3")) && bitrate == TTK_BN_192)
    {
        quality = "192kmp3";
    }
    else if((format.contains("MP3H") || format.contains("320kmp3")) && bitrate == TTK_BN_320 )
    {
        quality = "320kmp3";
    }
    else if((format.contains("FLAC") || format.contains("2000kflac")) && bitrate == TTK_BN_1000)
    {
        quality = "2000kflac";
    }
    else
    {
        return;
    }

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_SONG_PATH_V1_URL, false).arg(info->m_songId, quality));
    ReqKWInterface::makeRequestRawHeader(&request);

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
                TTK::MusicSongProperty prop;
                prop.m_bitrate = bitrate;
                prop.m_format = suffix;
                prop.m_size = TTK_DEFAULT_STR;
                prop.m_url = value["url"].toString();
                info->m_songProps.append(prop);
            }
        }
    }
}

static void parseSongPropertyV2(TTK::MusicSongInformation *info, const QString &format, int bitrate)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == bitrate)
        {
            return;
        }
    }

    TTK_INFO_STREAM("parse song property in v2 module");

    QString quality;
    if((format.contains("MP3128") || format.contains("128kmp3")) && bitrate == TTK_BN_128)
    {
        quality = "128";
    }
    else if((format.contains("MP3H") || format.contains("320kmp3")) && bitrate == TTK_BN_320 )
    {
        quality = "320";
    }
    else if((format.contains("FLAC") || format.contains("2000kflac")) && bitrate == TTK_BN_1000)
    {
        quality = "flac";
    }
    else
    {
        return;
    }

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_SONG_PATH_V2_URL, false));
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    const QByteArray &parameter = TTK::Algorithm::mdII(KW_SONG_PATH_V2_DATA_URL, false).arg(info->m_songId, quality).toUtf8();
    const QByteArray &bytes = TTK::syncNetworkQueryForPost(&request, parameter);
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
            TTK::MusicSongProperty prop;
            prop.m_bitrate = bitrate;
            prop.m_format = bitrate > TTK_BN_320 ? FLAC_FILE_SUFFIX : MP3_FILE_SUFFIX;
            prop.m_size = TTK_DEFAULT_STR;
            prop.m_url = value["data"].toString();
            info->m_songProps.append(prop);
        }
    }
}

static void parseSongPropertyV3(TTK::MusicSongInformation *info, const QString &format, int bitrate)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == bitrate)
        {
            return;
        }
    }

    TTK_INFO_STREAM("parse song property in v3 module");

    QString quality;
    if((format.contains("MP3128") || format.contains("128kmp3")) && bitrate == TTK_BN_128)
    {
        quality = "128k";
    }
    else if((format.contains("MP3H") || format.contains("320kmp3")) && bitrate == TTK_BN_320 )
    {
        quality = "320k";
    }
    else if((format.contains("FLAC") || format.contains("2000kflac")) && bitrate == TTK_BN_1000)
    {
        quality = "flac";
    }
    else
    {
        return;
    }

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_SONG_PATH_V3_URL, false).arg("kw", info->m_songId, quality));
    TTK::setSslConfiguration(&request);

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
        if(value["code"].toInt() == 0 && value.contains("data"))
        {
            TTK::MusicSongProperty prop;
            prop.m_bitrate = bitrate;
            prop.m_format = bitrate > TTK_BN_320 ? FLAC_FILE_SUFFIX : MP3_FILE_SUFFIX;
            prop.m_size = TTK_DEFAULT_STR;
            prop.m_url = value["data"].toString();

            value = value["extra"].toMap();
            if(value.isEmpty())
            {
                return;
            }

            value = value["quality"].toMap();
            if(value.isEmpty())
            {
                return;
            }

            if(value["target"].toString() == quality && value["result"].toString() == quality)
            {
                info->m_songProps.append(prop);
            }
        }
    }
}

static void parseSongProperty(TTK::MusicSongInformation *info, const QString &suffix, const QString &format, int bitrate)
{
    parseSongPropertyV1(info, suffix, format, bitrate);
    parseSongPropertyV2(info, format, bitrate);
    parseSongPropertyV3(info, format, bitrate);
}

void ReqKWInterface::parseFromSongProperty(TTK::MusicSongInformation *info, int bitrate)
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    if(info->m_formatProps.isEmpty())
    {
        parseSongProperty(info, MP3_FILE_SUFFIX, "128kmp3", TTK_BN_128);
        return;
    }

    if(bitrate == TTK_BN_0)
    {
        parseSongProperty(info, MP3_FILE_SUFFIX, info->m_formatProps, TTK_BN_128);
        parseSongProperty(info, MP3_FILE_SUFFIX, info->m_formatProps, TTK_BN_192);
        parseSongProperty(info, MP3_FILE_SUFFIX, info->m_formatProps, TTK_BN_320);
        parseSongProperty(info, FLAC_FILE_SUFFIX, info->m_formatProps, TTK_BN_1000);
    }
    else
    {
        parseSongProperty(info, bitrate > TTK_BN_320 ? FLAC_FILE_SUFFIX : MP3_FILE_SUFFIX, info->m_formatProps, bitrate);
    }
}

void ReqKWInterface::parseFromSongProperty(TTK::MusicSongInformation *info, const QString &format)
{
    info->m_formatProps = format;
}
