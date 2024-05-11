#include "musiclxqueryinterface.h"
#include "musicabstractqueryrequest.h"

namespace ReqLXInterface
{
    //QSszbjQ4WXNpUjVJVElVNnZFSER5TUZCaTBTQzJqdnE1MUg5eUxRTS8yZU1sZzdxRGgvQnBHdmQzOFNDTnBsZHc2RUtBb0Q0OUF4K3IrTG0=
    static constexpr const char *IKU_BASE_URL = "RWx2TXJmVWFSM3NGMW5NWFJackIzRlBHZUg3NW4wclB2a0hHSktoYm82SFdFZUhzdVYvVzYxYysvb0VEL0FCcGdkaFJrZz09";
    //dW1rcVZlTDFpRFhkNXBLQVNLM3dxY3p0RUp5T3VXeFZ6V0lNSDJnNmhzcExLRnEzKzNjT3VOSjdtbW5LeGNrMis4SFI0UDM5MHlNanRnV2RLdzV4K21hRGFFN0x6TWVM
    static constexpr const char *HBQ_BASE_URL = "c1kxMVdpRm15cmRnbzdOU1lZY1FPMkRjWDFiRDlNeXRTQ2t1T0FvdGRPRXlBWFpLcmJDeURRPT0=";
}

static void parseSongPropertyIKU(TTK::MusicSongInformation *info, const QString &type, const QString &quality, int bitrate)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == bitrate)
        {
            return;
        }
    }

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(ReqLXInterface::IKU_BASE_URL, false).arg(type, info->m_songId, quality));
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

static void parseSongPropertyHBQ(TTK::MusicSongInformation *info, const QString &type, const QString &quality, int bitrate)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == bitrate)
        {
            return;
        }
    }

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(ReqLXInterface::HBQ_BASE_URL, false).arg(type, info->m_songId, quality));
    TTK::setSslConfiguration(&request);
    request.setRawHeader("X-Request-Key", TTK::Algorithm::mdII("YkhWWUNCaFJCM3lLUVhRNA==", false).toUtf8());

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
        if(value["code"].toInt() == 0 && value.contains("url"))
        {
            TTK::MusicSongProperty prop;
            prop.m_bitrate = bitrate;
            prop.m_format = bitrate > TTK_BN_320 ? FLAC_FILE_SUFFIX : MP3_FILE_SUFFIX;
            prop.m_size = TTK_DEFAULT_STR;
            prop.m_url = value["url"].toString();

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


void ReqLXInterface::parseFromSongProperty(TTK::MusicSongInformation *info, const QString &suffix, int bitrate)
{
    QString type;
    if(suffix == QUERY_WY_INTERFACE)
    {
        type = "wy";
    }
    else if(suffix == QUERY_KW_INTERFACE)
    {
        type = "kw";
    }
    else if(suffix == QUERY_KG_INTERFACE)
    {
        type = "kg";
    }

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

    if(type.isEmpty() || quality.isEmpty())
    {
        return;
    }

    parseSongPropertyIKU(info, type, quality, bitrate);
    parseSongPropertyHBQ(info, type, quality, bitrate);
}
