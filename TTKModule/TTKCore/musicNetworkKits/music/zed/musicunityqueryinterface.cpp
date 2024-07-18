#include "musicunityqueryinterface.h"

//Vlc3dFJiZVZzS096RW13QmJIQms1cUhxaXJkcHFjZkwxRUFMUmlhZWhiZkd4TFUyYnRxNzdnPT0=
static constexpr const char *OS_PLUGINS_URL = "resource/plugins";

static void parseSongPropertyV1(TTK::MusicSongInformation *info, const QString &url, const QString &quality, int bitrate)
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

void ReqUnityInterface::parseFromSongProperty(TTK::MusicSongInformation *info, const QString &type, const QString &id, const QString &quality, int bitrate)
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
            for(const QString &key : value.keys())
            {
                if(key == "A")
                {
                    const QString &url = value[key].toString();
                    if(url.isEmpty())
                    {
                        continue;
                    }

                    parseSongPropertyV1(info, TTK::Algorithm::mdII(url, false).arg(type, id, quality), quality, bitrate);
                }
                else if(key == "B")
                {
                    // TODO
                }
            }
        }
    }
}
