#include "musicunityqueryinterface.h"

static constexpr const char *QUERY_PLUGINS_URL = "resource/plugins";
static constexpr const char *QUERY_MODULE_A = "A";
static constexpr const char *QUERY_MODULE_B = "B";

static QString makeQualityValue(const QString &type, int bitrate)
{
    if(type == QUERY_MODULE_A)
    {
        switch(bitrate)
        {
        case TTK_BN_128: return "128k";
        case TTK_BN_320: return "320k";
        case TTK_BN_1000: return "flac";
        default: return {};
        }
    }
    else if(type == QUERY_MODULE_B)
    {
        switch(bitrate)
        {
        case TTK_BN_128: return "128";
        case TTK_BN_320: return "320";
        case TTK_BN_1000: return "999";
        default: return {};
        }
    }
    return {};
}

static QString makeModuleValue(const QString &type, const QString &module)
{
    if(type == QUERY_MODULE_A)
    {
        if(module == QUERY_WY_INTERFACE) return "wy";
        else if(module == QUERY_KG_INTERFACE) return "kg";
        else if(module == QUERY_KW_INTERFACE) return "kw";
        else return {};
    }
    else if(type == QUERY_MODULE_B)
    {
        if(module == QUERY_WY_INTERFACE) return "wangyi";
        else if(module == QUERY_KG_INTERFACE) return "kugou";
        else if(module == QUERY_KW_INTERFACE) return "kuwo";
        else return {};
    }
    return {};
}

//OVpCbTNnaHljODBhZ0hHUzNvejBGTmFJREdXVzRWWDJwREw4ckNSZGNrbz0=
//Ukg1OFg2VUIzWDVQbWxYSkdYUHY1d21mOVZ3SlpERHhFYjFyQzREb29NTUFTZW55eVN1YVRnPT0=
static void parseSongPropertyA(TTK::MusicSongInformation *info, const QString &key, const QString &url, const QString &quality, int bitrate)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == bitrate)
        {
            return;
        }
    }

    TTK_INFO_STREAM("Parse song in A module, url:" << url);

    QNetworkRequest request;
    request.setRawHeader("X-Request-Key", TTK::Algorithm::mdII(key, false).toUtf8());
    request.setUrl(url);
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
        if(value["code"].toInt() == 0)
        {
            TTK::MusicSongProperty prop;
            prop.m_url = value["data"].toString();
            prop.m_size = TTK_DEFAULT_STR;
            prop.m_format = bitrate > TTK_BN_320 ? FLAC_FILE_SUFFIX : MP3_FILE_SUFFIX;
            prop.m_bitrate = bitrate;

            if(prop.isEmpty())
            {
                prop.m_url = value["url"].toString();
            }

            if(prop.isEmpty())
            {
                return;
            }

            const QVariantMap &extra = value["extra"].toMap();
            if(!extra.isEmpty())
            {
                value = extra["extra"].toMap();
            }

            value = value["quality"].toMap();
            if(value.isEmpty())
            {
                return;
            }

            if(value["target"].toString().contains(quality, Qt::CaseInsensitive) && value["result"].toString().contains(quality, Qt::CaseInsensitive))
            {
                info->m_songProps.append(prop);
            }
        }
    }
}

//aFB0VHBaY3lpMHNKWTBjVk5Dd3JrZlpkeVRYamhnNDVPVHR0dlZhSTFDVW5rWm5PVjJtcWhrWVJweU09
static void parseSongPropertyB(TTK::MusicSongInformation *info, const QString &url, int bitrate)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == bitrate)
        {
            return;
        }
    }

    TTK_INFO_STREAM("Parse song in B module, url:" << url);

    QNetworkRequest request;
    request.setUrl(url);
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
        if(value.contains("url"))
        {
            TTK::MusicSongProperty prop;
            prop.m_url = value["url"].toString();
            prop.m_size = TTK_DEFAULT_STR;
            prop.m_format = bitrate > TTK_BN_320 ? FLAC_FILE_SUFFIX : MP3_FILE_SUFFIX;
            prop.m_bitrate = bitrate;

            if(!prop.isEmpty())
            {
                info->m_songProps.append(prop);
            }
        }
    }
}


void ReqUnityInterface::parseFromSongProperty(TTK::MusicSongInformation *info, const QString &type, const QString &id, int bitrate)
{
    QByteArray bytes;
    QFile file(APPCACHE_DIR_FULL + QUERY_PLUGINS_URL);
    if(file.open(QIODevice::ReadOnly))
    {
        TTK_INFO_STREAM("Load server unity plugins using local resource config");
        bytes = file.readAll();
        file.close();
    }
    else
    {
        TTK_ERROR_STREAM("Load server unity plugins resource config failed");
        return;
    }

    QJson::Parser json;
    bool ok = false;
    const QVariant &data = json.parse(bytes, &ok);
    if(ok)
    {
        const QVariantList &datas = data.toList();
        for(const QVariant &var : qAsConst(datas))
        {
            if(var.isNull())
            {
                continue;
            }

            const QVariantMap &value = var.toMap();
            const QString &key = value["key"].toString();
            const bool option = value["option"].toBool();

            if(!option)
            {
                continue;
            }

            for(const QString &module : value.keys())
            {
                const QString &url = value[module].toString();
                if(url.isEmpty())
                {
                    continue;
                }

                const QString &server = makeModuleValue(module, type);
                const QString &quality = makeQualityValue(module, bitrate);

                if(server.isEmpty() || quality.isEmpty())
                {
                    continue;
                }

                if(module == QUERY_MODULE_A)
                {
                    parseSongPropertyA(info, key, TTK::Algorithm::mdII(url, false).arg(server, id, quality), quality, bitrate);
                }
                else if(module == QUERY_MODULE_B)
                {
                    parseSongPropertyB(info, TTK::Algorithm::mdII(url, false).arg(server, id, quality), bitrate);
                }
            }
        }
    }
}
