#include "musickwqueryinterface.h"
#include "musicunityqueryinterface.h"

#include "qalgorithm/deswrapper.h"

static constexpr const char *KW_SONG_PATH_V1_URL = "eVZwd0d3aGhUSmNoNHBHZTFFYldyOGZjY0trcldsMlphUWtNL0thR3AyTjRkU0xyZGJuSlBGQTBueURhaDM3bTVrNUE5S0I5dWpjcWRHeTNaanJPUXc1YkNOQm9pNlJrbVFFTWI3YVdpcFFWS2tUSGNoSk5RUDl3WDhSZlZaUzd2TnRpaDBTS0psbVE0Tmk5YlJHQXdtY2tGaGlERmU4RnFXSGVZdWFWeGo4dUFpbm4zWUJTemlFYVhkREM0Q0pSNytUK05VYlBVcUdVRnhLSUJ3TXg3ZEc2UzBxVGVMOFZqdnJJN1E9PQ==";
static constexpr const char *KW_SONG_PATH_V2_URL = "VlpjeWIxQkZNbDdkNHA3eUFRMHVnNmVVanpITWJ6MEp1T1ZraElNc2JXaGIvVytMcmFNZVFKNEQ2UXRrcHZyV2dXTjJqQT09";
static constexpr const char *KW_SONG_PATH_V2_DATA_URL = "NUVnZE5SbWFmNHRGemdTeHd6RHlwYjF2TllaR1R5TkNKRUt6NWpxcDVEcW9JdkREN0QzRmNGOVM0R0lQK29wTTZwaHBHa3FkR3dWbEx6M3ZHVnZBM1NseWF1eUoyN0wwZmQwT3BJaXFzbWcraDlISnAyVnN0VTdGT3ZxZWlicVFGcjFkYWc3QlJ0czlGdFVrTW1WeTd3U1c1S0wwQjRrNDM0S0EySU9pOVg4QmVYSW5GUUIwWWcwMlM5Sng2YVIvYmpYL0xib2NnRlFLV3A2aXBJMWxEalNMNVpnPQ==";
static constexpr const char *KW_SONG_PATH_V3_URL = "UElteEluQm8zUnRXTERvMkkwYTVhNitEUG9rRVA0Wk80VFFOb3dKL1ZsNldLc1RDVFJQZ3llaTN1MXhzL3NSM3ovaVhTTld5L1NpTE9hdTlBM3NuNjBQbWpOTT0=";
static constexpr const char *KW_ALBUM_COVER_URL = "NkhRaDluWTFxV2wvTEl6ZkszRmRUa1MvS0JlQUl6MDJVUkpqcjFnYzJ4djZ0b0xmcjhzZHJnV0xscW89";
static constexpr const char *KW_ARTIST_COVER_URL = "YW4xU3FFQnprWG11R0J5QjVGREFHdy9Yd0w3dEZZcFJIWm5FMXQ5QkJJN0EzQTd4TnBwR1BQZ0d5Qzg9";

void ReqKWInterface::makeRequestRawHeader(QNetworkRequest *request) noexcept
{
    TTK::setSslConfiguration(request);
    TTK::setUserAgentHeader(request);
    TTK::setContentTypeHeader(request);
}

QString ReqKWInterface::makeSongArtist(const QString &name)
{
    return TTK::String::charactersReplace(name).replace("&", ";").replace("+", ";");
}

QString ReqKWInterface::makeSongArtist(const QString &in, const QString &name)
{
    const QString &artistName = TTK::String::charactersReplace(name);
    return in.isEmpty() ? artistName : (in + ";" + artistName);
}

QString ReqKWInterface::makeArtistPixmapUrl(const QString &url)
{
    return TTK::Algorithm::mdII(KW_ARTIST_COVER_URL, false) + url.section('/', 1);
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

    TTK_INFO_STREAM("parse song" << bitrate << "kbps property in v1 module");

    QString quality;
    if((format.contains("MP3128") || format.contains("128kmp3")) && bitrate == TTK_BN_128)
    {
        quality = "128kmp3";
    }
    else if((format.contains("MP3192") || format.contains("192kmp3")) && bitrate == TTK_BN_192)
    {
        quality = "192kmp3";
    }
    else if((format.contains("MP3H") || format.contains("320kmp3")) && bitrate == TTK_BN_320)
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

    QJsonParseError ok;
    const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
    if(QJsonParseError::NoError == ok.error)
    {
        QVariantMap value = json.toVariant().toMap();
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
                prop.m_url = value["url"].toString();
                prop.m_size = TTK_DEFAULT_STR;
                prop.m_format = suffix;
                prop.m_bitrate = bitrate;
                info->m_songProps.append(prop);
            }
        }
    }
}

static void parseSongPropertyV2(TTK::MusicSongInformation *info, const QString &suffix, const QString &format, int bitrate)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == bitrate)
        {
            return;
        }
    }

    TTK_INFO_STREAM("parse song" << bitrate << "kbps property in v2 module");

    QString quality;
    if((format.contains("MP3128") || format.contains("128kmp3")) && bitrate == TTK_BN_128)
    {
        quality = "128kmp3";
    }
    else if((format.contains("MP3192") || format.contains("192kmp3")) && bitrate == TTK_BN_192)
    {
        quality = "192kmp3";
    }
    else if((format.contains("MP3H") || format.contains("320kmp3")) && bitrate == TTK_BN_320)
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

    QAlgorithm::Des des;
    const QString &parameter = des.encrypt(TTK::Algorithm::mdII(KW_SONG_PATH_V2_DATA_URL, false).arg(quality).toUtf8(),
                                           TTK::Algorithm::mdII("OGlVTjJWOEdlMkkzSkZIeg==", MDII_SHR_KEY, false).toUtf8());
    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_SONG_PATH_V2_URL, false).arg(parameter, info->m_songId));
    ReqKWInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    if(!bytes.contains("res not found"))
    {
        const QString text(bytes);
        static TTKRegularExpression regx(".*url=(.*)\r\nsig=");

        if(regx.match(text) != -1)
        {
            TTK::MusicSongProperty prop;
            prop.m_url = regx.captured(1);
            prop.m_size = TTK_DEFAULT_STR;
            prop.m_format = suffix;
            prop.m_bitrate = bitrate;

            if(prop.isEmpty() || info->m_songProps.contains(prop))
            {
                return;
            }

            info->m_songProps.append(prop);
        }
    }
}

static void parseSongPropertyV3(TTK::MusicSongInformation *info, const QString &suffix, const QString &format, int bitrate)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == bitrate)
        {
            return;
        }
    }

    TTK_INFO_STREAM("parse song" << bitrate << "kbps property in v3 module");

    QString quality;
    if((format.contains("MP3128") || format.contains("128kmp3")) && bitrate == TTK_BN_128)
    {
        quality = "128k";
    }
    else if((format.contains("MP3192") || format.contains("192kmp3")) && bitrate == TTK_BN_192)
    {
        quality = "192k";
    }
    else if((format.contains("MP3H") || format.contains("320kmp3")) && bitrate == TTK_BN_320)
    {
        quality = "320k";
    }
    else if((format.contains("FLAC") || format.contains("2000kflac")) && bitrate == TTK_BN_1000)
    {
        quality = "lossless";
    }
    else
    {
        return;
    }

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_SONG_PATH_V3_URL, false).arg(info->m_songId, quality));
    ReqKWInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    QJsonParseError ok;
    const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
    if(QJsonParseError::NoError == ok.error)
    {
        QVariantMap value = json.toVariant().toMap();
        if(value["code"].toInt() == 200 && value.contains("data"))
        {
            value = value["data"].toMap();
            if(value.isEmpty())
            {
                return;
            }

            TTK::MusicSongProperty prop;
            prop.m_url = value["url"].toString();
            prop.m_size = value["size"].toString();
            prop.m_format = suffix;
            prop.m_bitrate = bitrate;
            info->m_songProps.append(prop);
        }
    }
}

static void parseSongPropertyUnity(TTK::MusicSongInformation *info, const QString &format, int bitrate)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == bitrate)
        {
            return;
        }
    }

    TTK_INFO_STREAM("parse song" << bitrate << "kbps property in unity module");

    if(((format.contains("MP3128") || format.contains("128kmp3")) && bitrate == TTK_BN_128) ||
       ((format.contains("MP3H") || format.contains("320kmp3")) && bitrate == TTK_BN_320) ||
       ((format.contains("FLAC") || format.contains("2000kflac")) && bitrate == TTK_BN_1000))
    {
        ReqUnityInterface::parseFromSongProperty(info, QUERY_KW_INTERFACE, info->m_songId, bitrate);
    }
}

static void parseSongProperty(TTK::MusicSongInformation *info, const QString &suffix, const QString &format, int bitrate)
{
    if(G_SETTING_PTR->value(MusicSettingManager::DownloadServerPriority).toBool())
    {
        parseSongPropertyUnity(info, format, bitrate);
        parseSongPropertyV1(info, suffix, format, bitrate);
        parseSongPropertyV2(info, suffix, format, bitrate);
        parseSongPropertyV3(info, suffix, format, bitrate);
    }
    else
    {
        parseSongPropertyV1(info, suffix, format, bitrate);
        parseSongPropertyV2(info, suffix, format, bitrate);
        parseSongPropertyV3(info, suffix, format, bitrate);
        parseSongPropertyUnity(info, format, bitrate);
    }
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
