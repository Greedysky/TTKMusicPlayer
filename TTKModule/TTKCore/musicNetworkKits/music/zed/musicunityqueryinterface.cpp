#include "musicunityqueryinterface.h"

static constexpr const char *QUERY_PLUGINS_URL = "resource/plugins";
static constexpr const char *QUERY_MODULE_X = "*";
static constexpr const char *QUERY_MODULE_A = "A";
static constexpr const char *QUERY_MODULE_B = "B";
static constexpr const char *QUERY_MODULE_C = "C";

struct ServerModule
{
    QString m_ua;
    QString m_url;
    QString m_key;
    QString m_quality;
    int m_bitrate;
};

static QString makeQualityValue(const QString &type, int bitrate) noexcept
{
    if(type == QUERY_MODULE_X || type == QUERY_MODULE_A || type == QUERY_MODULE_C)
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

static QString makeModuleValue(const QString &type, const QString &module) noexcept
{
    if(type == QUERY_MODULE_X || type == QUERY_MODULE_A || type == QUERY_MODULE_C)
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

static void parseSongPropertyX(TTK::MusicSongInformation *info, const ServerModule &module)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == module.m_bitrate)
        {
            return;
        }
    }

    TTK_INFO_STREAM("Parse song in X module, url:" << module.m_url);

    QNetworkRequest request;
    if(!module.m_ua.isEmpty())
    {
        request.setRawHeader("User-Agent", TTK::Algorithm::mdII(module.m_ua, false).toUtf8());
    }
    request.setRawHeader("X-Request-Key", TTK::Algorithm::mdII(module.m_key, false).toUtf8());
    request.setUrl(module.m_url);
    TTK::setSslConfiguration(&request);

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
        if(value["code"].toInt() == 0 || value["code"].toInt() == 200)
        {
            TTK::MusicSongProperty prop;
            prop.m_url = value["data"].toString();
            prop.m_size = TTK_DEFAULT_STR;
            prop.m_format = module.m_bitrate > TTK_BN_320 ? FLAC_FILE_SUFFIX : MP3_FILE_SUFFIX;
            prop.m_bitrate = module.m_bitrate;

            if(prop.isEmpty())
            {
                prop.m_url = value["url"].toString();
                if(prop.isEmpty())
                {
                    return;
                }
            }

            info->m_songProps.append(prop);
        }
    }
}

static void parseSongPropertyA(TTK::MusicSongInformation *info, const ServerModule &module)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == module.m_bitrate)
        {
            return;
        }
    }

    TTK_INFO_STREAM("Parse song in A module, url:" << module.m_url);

    QNetworkRequest request;
    if(!module.m_ua.isEmpty())
    {
        request.setRawHeader("User-Agent", TTK::Algorithm::mdII(module.m_ua, false).toUtf8());
    }
    request.setRawHeader("X-Request-Key", TTK::Algorithm::mdII(module.m_key, false).toUtf8());
    request.setUrl(module.m_url);
    TTK::setSslConfiguration(&request);

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
        if(value["code"].toInt() == 0 || value["code"].toInt() == 200)
        {
            TTK::MusicSongProperty prop;
            prop.m_url = value["data"].toString();
            prop.m_size = TTK_DEFAULT_STR;
            prop.m_format = module.m_bitrate > TTK_BN_320 ? FLAC_FILE_SUFFIX : MP3_FILE_SUFFIX;
            prop.m_bitrate = module.m_bitrate;

            if(prop.isEmpty())
            {
                prop.m_url = value["url"].toString();
                if(prop.isEmpty())
                {
                    return;
                }
            }

            const QVariantMap &extra = value["extra"].toMap();
            if(!extra.isEmpty())
            {
                value = extra;
            }

            const QVariantMap &quality = value["quality"].toMap();
            if(quality["target"].toString().contains(module.m_quality, Qt::CaseInsensitive) &&
               quality["result"].toString().contains(module.m_quality, Qt::CaseInsensitive))
            {
                info->m_songProps.append(prop);
            }
        }
    }
}

static void parseSongPropertyB(TTK::MusicSongInformation *info, const ServerModule &module)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == module.m_bitrate)
        {
            return;
        }
    }

    TTK_INFO_STREAM("Parse song in B module, url:" << module.m_url);

    QNetworkRequest request;
    if(!module.m_ua.isEmpty())
    {
        request.setRawHeader("User-Agent", TTK::Algorithm::mdII(module.m_ua, false).toUtf8());
    }
    request.setUrl(module.m_url);
    TTK::setSslConfiguration(&request);

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
        if(value.contains("url"))
        {
            TTK::MusicSongProperty prop;
            prop.m_url = value["url"].toString();
            prop.m_size = value.contains("size") ?  TTK::Number::sizeByteToLabel(value["size"].toInt()) : TTK_DEFAULT_STR;
            prop.m_format = module.m_bitrate > TTK_BN_320 ? FLAC_FILE_SUFFIX : MP3_FILE_SUFFIX;
            prop.m_bitrate = value.contains("br") ? value["br"].toInt() : module.m_bitrate;

            if(!prop.isEmpty())
            {
                info->m_songProps.append(prop);
            }
        }
    }
}

static void parseSongPropertyC(TTK::MusicSongInformation *info, const ServerModule &module)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == module.m_bitrate)
        {
            return;
        }
    }

    TTK_INFO_STREAM("Parse song in C module, url:" << module.m_url);

    QNetworkRequest request;
    if(!module.m_ua.isEmpty())
    {
        request.setRawHeader("User-Agent", TTK::Algorithm::mdII(module.m_ua, false).toUtf8());
    }
    request.setRawHeader("X-Request-Key", TTK::Algorithm::mdII(module.m_key, false).toUtf8());
    request.setUrl(module.m_url);
    TTK::setSslConfiguration(&request);

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
        if(value["code"].toInt() == 0 || value["code"].toInt() == 200)
        {
            TTK::MusicSongProperty prop;
            prop.m_url = value["url"].toString();
            prop.m_size = TTK_DEFAULT_STR;
            prop.m_format = module.m_bitrate > TTK_BN_320 ? FLAC_FILE_SUFFIX : MP3_FILE_SUFFIX;
            prop.m_bitrate = module.m_bitrate;

            if(prop.isEmpty())
            {
                prop.m_url = value["data"].toString();
                if(prop.isEmpty())
                {
                    return;
                }
            }

            const QString &quality = value["quality"].toString();
            if(quality.contains(module.m_quality, Qt::CaseInsensitive))
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

    QJsonParseError ok;
    const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
    if(QJsonParseError::NoError == ok.error)
    {
        const QVariantList &datas = json.toVariant().toList();
        for(const QVariant &var : qAsConst(datas))
        {
            if(var.isNull())
            {
                continue;
            }

            const QVariantMap &value = var.toMap();
            const bool option = value["option"].toBool();

            if(!option)
            {
                continue;
            }

            const QString &ua = value["ua"].toString();
            const QString &key = value["key"].toString();

            for(const QString &module : value.keys())
            {
                ServerModule v;
                v.m_url = value[module].toString();
                if(v.m_url.isEmpty())
                {
                    continue;
                }

                const QString &server = makeModuleValue(module, type);
                const QString &quality = makeQualityValue(module, bitrate);
                if(server.isEmpty() || quality.isEmpty())
                {
                    continue;
                }

                v.m_ua = ua;
                v.m_key = key;
                v.m_quality = quality;
                v.m_bitrate = bitrate;
                v.m_url = TTK::Algorithm::mdII(v.m_url, false).arg(server, id, quality);

                if(module == QUERY_MODULE_X)
                {
                    parseSongPropertyX(info, v);
                }
                else if(module == QUERY_MODULE_A)
                {
                    parseSongPropertyA(info, v);
                }
                else if(module == QUERY_MODULE_B)
                {
                    parseSongPropertyB(info, v);
                }
                else if(module == QUERY_MODULE_C)
                {
                    parseSongPropertyC(info, v);
                }
            }
        }
    }
}
