#include "musicwyqueryinterface.h"
#include "musicurlutils.h"
#include "musicabstractnetwork.h"

#include "qalgorithm/aeswrapper.h"

void MusicWYInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    request->setRawHeader("Referer", TTK::Algorithm::mdII(WY_BASE_URL, false).toUtf8());
    request->setRawHeader("Origin", TTK::Algorithm::mdII(WY_BASE_URL, false).toUtf8());
    request->setRawHeader("User-Agent", TTK::Algorithm::mdII(WY_UA_URL, ALG_UA_KEY, false).toUtf8());
    request->setRawHeader("Cookie", QString("MUSIC_U=%1; NMTID=%2; ").arg(TTK::Algorithm::mdII(WY_COOKIE_URL, false),
                                            TTK::Algorithm::mdII(WY_NMTID_URL, ALG_UA_KEY, false)).toUtf8());
    TTK::setSslConfiguration(request);
}

QByteArray MusicWYInterface::makeTokenRequest(QNetworkRequest *request, const QString &query, const QString &type)
{
    QAlgorithm::Aes aes;
    QByteArray parameter = aes.encryptCBC(type.toUtf8(), "0CoJUm6Qyw8W8jud", "0102030405060708");
    parameter = aes.encryptCBC(parameter, "a44e542eaac91dce", "0102030405060708");
    TTK::Url::urlEncode(parameter);

    request->setUrl(query);
    MusicWYInterface::makeRequestRawHeader(request);

    return "params=" + parameter + "&encSecKey=" + WY_SECKRY_STRING.toUtf8();
}

void MusicWYInterface::parseFromSongProperty(TTK::MusicSongInformation *info, int bitrate)
{
    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(WY_SONG_INFO_OLD_URL, false).arg(bitrate * 1000).arg(info->m_songId));
    MusicWYInterface::makeRequestRawHeader(&request);

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

            TTK::MusicSongProperty prop;
            prop.m_url = value["url"].toString();
            prop.m_bitrate = bitrate;

            if(prop.m_url.isEmpty())
            {
                parseFromSongPropertyNew(info, bitrate);
                return;
            }

            if(info->m_songProps.contains(prop))
            {
                return;
            }

            prop.m_size = TTK::Number::sizeByteToLabel(value["size"].toInt());
            prop.m_format = value["type"].toString();
            info->m_songProps.append(prop);
        }
    }
}

void MusicWYInterface::parseFromSongProperty(TTK::MusicSongInformation *info, const QVariantMap &key, TTK::QueryQuality quality, bool all)
{
    int maxBr = MB_1000;
    const QVariantMap &privilege = key["privilege"].toMap();
    if(!privilege.isEmpty())
    {
        const QString &brStr = privilege["maxbr"].toString();
        if(brStr == "999000")
        {
            maxBr = MB_1000;
        }
        else if(brStr == "320000" || brStr == "192000" || brStr == "190000")
        {
            maxBr = MB_320;
        }
        else if(brStr == "160000")
        {
            maxBr = MB_192;
        }
        else
        {
            maxBr = MB_128;
        }
    }

    if(all)
    {
        if(maxBr == MB_1000)
        {
            parseFromSongProperty(info, MB_128);
            parseFromSongProperty(info, MB_192);
            parseFromSongProperty(info, MB_320);
            parseFromSongProperty(info, MB_1000);
        }
        else if(maxBr == MB_320)
        {
            parseFromSongProperty(info, MB_128);
            parseFromSongProperty(info, MB_192);
            parseFromSongProperty(info, MB_320);
        }
        else if(maxBr == MB_192)
        {
            parseFromSongProperty(info, MB_128);
            parseFromSongProperty(info, MB_192);
        }
        else
        {
            parseFromSongProperty(info, MB_128);
        }
    }
    else
    {
        if(quality == TTK::QueryQuality::Standard && maxBr >= MB_128)
        {
            parseFromSongProperty(info, MB_128);
        }
        else if(quality == TTK::QueryQuality::High && maxBr >= MB_192)
        {
            parseFromSongProperty(info, MB_192);
        }
        else if(quality == TTK::QueryQuality::Super && maxBr >= MB_320)
        {
            parseFromSongProperty(info, MB_320);
        }
        else if(quality == TTK::QueryQuality::Lossless && maxBr >= MB_1000)
        {
            parseFromSongProperty(info, MB_1000);
        }
    }
}

void MusicWYInterface::parseFromSongPropertyNew(TTK::MusicSongInformation *info, int bitrate)
{
    QNetworkRequest request;
    const QByteArray &parameter = makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_SONG_PATH_URL, false),
                      TTK::Algorithm::mdII(WY_SONG_PATH_DATA_URL, false).arg(info->m_songId).arg(bitrate * 1000));

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
            const QVariantList &datas = value["data"].toList();
            for(const QVariant &var : qAsConst(datas))
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();

                TTK::MusicSongProperty prop;
                prop.m_url = value["url"].toString();
                prop.m_bitrate = bitrate;

                if(prop.m_url.isEmpty() || info->m_songProps.contains(prop))
                {
                    break;
                }

                prop.m_size = TTK::Number::sizeByteToLabel(value["size"].toInt());
                prop.m_format = value["type"].toString();
                info->m_songProps.append(prop);
            }
        }
    }
}
