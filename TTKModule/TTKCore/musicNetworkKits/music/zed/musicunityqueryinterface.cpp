#include "musicunityqueryinterface.h"

static constexpr const char *QUERY_MODULE_X = "*";
static constexpr const char *QUERY_MODULE_A = "A";
static constexpr const char *QUERY_MODULE_B = "B";
static constexpr const char *QUERY_MODULE_C = "C";
static constexpr const char *QUERY_MODULE_D = "D";
static constexpr const char *QUERY_PLUGINS_URL = "resource/plugins";

static bool checkModule(const QString &module)
{
    return module == QUERY_MODULE_X ||
           module == QUERY_MODULE_A ||
           module == QUERY_MODULE_B ||
           module == QUERY_MODULE_C ||
           module == QUERY_MODULE_D;
}


struct Data
{
    QString m_ua;
    QString m_url;
    QString m_quality;
    QString m_module;
};

static void parseSongPropertyX(QNetworkRequest *request, TTK::MusicSongInformation *info, int bitrate)
{
    const QByteArray &bytes = TTK::syncNetworkQueryForGet(request);
    if(bytes.isEmpty())
    {
        return;
    }

    bool found = false;
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
            prop.m_format = bitrate > TTK_BN_320 ? FLAC_FILE_SUFFIX : MP3_FILE_SUFFIX;
            prop.m_bitrate = bitrate;

            if(prop.isEmpty())
            {
                prop.m_url = value["url"].toString();
                if(prop.isEmpty())
                {
                    return;
                }
            }

            found = true;
            info->m_songProps.append(prop);
        }
    }

    if(!found)
    {
        TTK_INFO_STREAM("Not found metainfo, buffer is" << bytes);
    }
}

static void parseSongPropertyA(QNetworkRequest *request, TTK::MusicSongInformation *info, int bitrate, const QString &quality)
{
    const QByteArray &bytes = TTK::syncNetworkQueryForGet(request);
    if(bytes.isEmpty())
    {
        return;
    }

    bool found = false;
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
            prop.m_format = bitrate > TTK_BN_320 ? FLAC_FILE_SUFFIX : MP3_FILE_SUFFIX;
            prop.m_bitrate = bitrate;

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

            const QVariantMap &q = value["quality"].toMap();
            if(q["target"].toString().contains(quality, Qt::CaseInsensitive) &&
               q["result"].toString().contains(quality, Qt::CaseInsensitive))
            {
                found = true;
                info->m_songProps.append(prop);
            }
        }
    }

    if(!found)
    {
        TTK_INFO_STREAM("Not found metainfo, buffer is" << bytes);
    }
}

static void parseSongPropertyB(QNetworkRequest *request, TTK::MusicSongInformation *info, int bitrate)
{
    const QByteArray &bytes = TTK::syncNetworkQueryForGet(request);
    if(bytes.isEmpty())
    {
        return;
    }

    bool found = false;
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
            prop.m_format = bitrate > TTK_BN_320 ? FLAC_FILE_SUFFIX : MP3_FILE_SUFFIX;
            prop.m_bitrate = value.contains("br") ? value["br"].toInt() : bitrate;

            if(!prop.isEmpty())
            {
                found = true;
                info->m_songProps.append(prop);
            }
        }
    }

    if(!found)
    {
        TTK_INFO_STREAM("Not found metainfo, buffer is" << bytes);
    }
}

static void parseSongPropertyC(QNetworkRequest *request, TTK::MusicSongInformation *info, int bitrate, const QString &quality)
{
    const QByteArray &bytes = TTK::syncNetworkQueryForGet(request);
    if(bytes.isEmpty())
    {
        return;
    }

    bool found = false;
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
            prop.m_format = bitrate > TTK_BN_320 ? FLAC_FILE_SUFFIX : MP3_FILE_SUFFIX;
            prop.m_bitrate = bitrate;

            if(prop.isEmpty())
            {
                prop.m_url = value["data"].toString();
                if(prop.isEmpty())
                {
                    return;
                }
            }

            const QString &q = value["quality"].toString();
            if(q.contains(quality, Qt::CaseInsensitive))
            {
                found = true;
                info->m_songProps.append(prop);
            }
        }
    }

    if(!found)
    {
        TTK_INFO_STREAM("Not found metainfo, buffer is" << bytes);
    }
}

static void parseSongPropertyD(QNetworkRequest *request, TTK::MusicSongInformation *info, int bitrate)
{
    const QByteArray &bytes = TTK::syncNetworkQueryForGet(request);
    if(bytes.isEmpty())
    {
        return;
    }

    bool found = true;
    TTK::MusicSongProperty prop;
    prop.m_url = bytes;
    prop.m_size = TTK_DEFAULT_STR;
    prop.m_format = bitrate > TTK_BN_320 ? FLAC_FILE_SUFFIX : MP3_FILE_SUFFIX;
    prop.m_bitrate = bitrate;
    info->m_songProps.append(prop);

    if(!found)
    {
        TTK_INFO_STREAM("Not found metainfo, buffer is" << bytes);
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

    bool found = false;
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

            for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
            {
                if(prop.m_bitrate == bitrate)
                {
                    continue;
                }
            }

            const QVariantMap &serverMap = value["server"].toMap();
            const QVariantMap &qualityMap = value["quality"].toMap();
            if(!serverMap.contains(type) || !qualityMap.contains(QString::number(bitrate)))
            {
                continue;
            }

            Data v;
            v.m_ua = value["ua"].toString();
            v.m_quality = qualityMap[QString::number(bitrate)].toString();

            for(const QString &module : value.keys())
            {
                if(!checkModule(module))
                {
                    continue;
                }

                v.m_module = module;
                v.m_url = TTK::Algorithm::mdII(value[module].toString(), false).arg(serverMap[type].toString(), id, v.m_quality);
            }

            if(v.m_url.isEmpty())
            {
                continue;
            }

            QNetworkRequest request;
            request.setUrl(v.m_url);
            TTK::setSslConfiguration(&request);

            const QVariantMap &headerMap = value["headers"].toMap();
            if(!headerMap.isEmpty())
            {
                for(const QString &header : headerMap.keys())
                {
                    request.setRawHeader(header.toUtf8(), TTK::Algorithm::mdII(headerMap[header].toString(), false).toUtf8());
                }
            }

            found = true;
            if(!v.m_ua.isEmpty())
            {
                request.setRawHeader("User-Agent", TTK::Algorithm::mdII(v.m_ua, false).toUtf8());
            }

            if(v.m_module == QUERY_MODULE_X)
            {
                TTK_INFO_STREAM("Parse song in X module, url:" << v.m_url);
                parseSongPropertyX(&request, info, bitrate);
            }
            else if(v.m_module == QUERY_MODULE_A)
            {
                TTK_INFO_STREAM("Parse song in A module, url:" << v.m_url);
                parseSongPropertyA(&request, info, bitrate, v.m_quality);
            }
            else if(v.m_module == QUERY_MODULE_B)
            {
                TTK_INFO_STREAM("Parse song in B module, url:" << v.m_url);
                parseSongPropertyB(&request, info, bitrate);
            }
            else if(v.m_module == QUERY_MODULE_C)
            {
                TTK_INFO_STREAM("Parse song in C module, url:" << v.m_url);
                parseSongPropertyC(&request, info, bitrate, v.m_quality);
            }
            else if(v.m_module == QUERY_MODULE_D)
            {
                TTK_INFO_STREAM("Parse song in D module, url:" << v.m_url);
                parseSongPropertyD(&request, info, bitrate);
            }
        }
    }

    if(!found)
    {
        TTK_INFO_STREAM("Not found available server unity plugins");
    }
}
