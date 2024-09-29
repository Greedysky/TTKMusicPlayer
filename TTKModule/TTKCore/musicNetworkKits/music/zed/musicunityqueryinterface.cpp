#include "musicunityqueryinterface.h"

static constexpr const char *OS_PLUGINS_URL = "resource/plugins";

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
            prop.m_bitrate = bitrate;
            prop.m_format = bitrate > TTK_BN_320 ? FLAC_FILE_SUFFIX : MP3_FILE_SUFFIX;
            prop.m_size = TTK_DEFAULT_STR;
            prop.m_url = value["url"].toString();
            info->m_songProps.append(prop);
        }
    }
}


void ReqUnityInterface::parseFromSongProperty(TTK::MusicSongInformation *info, const QString &type, const QString &id, int bitrate)
{
    QByteArray bytes;
    QFile file(APPCACHE_DIR_FULL + OS_PLUGINS_URL);
    if(file.open(QIODevice::ReadOnly))
    {
        TTK_ERROR_STREAM("Load unity plugins using local resource config");
        bytes = file.readAll();
        file.close();
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

            for(const QString &module : value.keys())
            {
                const QString &url = value[module].toString();
                if(url.isEmpty())
                {
                    continue;
                }

                if(module == "A")
                {
                    QString quality;
                    if(bitrate == TTK_BN_128)
                    {
                        quality = "128k";
                    }
                    else if(bitrate == TTK_BN_320)
                    {
                        quality = "320k";
                    }
                    else if(bitrate == TTK_BN_1000)
                    {
                        quality = "flac";
                    }
                    else
                    {
                        continue;
                    }

                    QString server;
                    if(type == QUERY_WY_INTERFACE)
                    {
                        server = "wy";
                    }
                    else if(type == QUERY_KG_INTERFACE)
                    {
                        server = "kg";
                    }
                    else if(type == QUERY_KW_INTERFACE)
                    {
                        server = "kw";
                    }
                    else
                    {
                        continue;
                    }

                    parseSongPropertyA(info, key, TTK::Algorithm::mdII(url, false).arg(server, id, quality), quality, bitrate);
                }
                else if(module == "B")
                {
                    QString quality;
                    if(bitrate == TTK_BN_128)
                    {
                        quality = "128";
                    }
                    else if(bitrate == TTK_BN_320)
                    {
                        quality = "320";
                    }
                    else if(bitrate == TTK_BN_1000)
                    {
                        quality = "999";
                    }
                    else
                    {
                        continue;
                    }

                    QString server;
                    if(type == QUERY_WY_INTERFACE)
                    {
                        server = "wangyi";
                    }
                    else if(type == QUERY_KG_INTERFACE)
                    {
                        server = "kugou";
                    }
                    else if(type == QUERY_KW_INTERFACE)
                    {
                        server = "kuwo";
                    }
                    else
                    {
                        continue;
                    }

                    parseSongPropertyB(info, TTK::Algorithm::mdII(url, false).arg(server, id, quality), bitrate);
                }
            }
        }
    }
}
