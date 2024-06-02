#include "musicwyqueryinterface.h"
#include "musicurlutils.h"
#include "musicabstractnetwork.h"

#include "qalgorithm/aeswrapper.h"

static constexpr const char *_128KBPS = "QXMyZkZJc2dIb1FOenJlTg==";
static constexpr const char *_320KBPS = "UThNR09kcDRXNG9qbG45Ng==";
static constexpr const char *_999KBPS = "VGF0djlKc01mL1QxM1pyNQ==";

void ReqWYInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    request->setRawHeader("Referer", TTK::Algorithm::mdII(WY_BASE_URL, false).toUtf8());
    request->setRawHeader("Origin", TTK::Algorithm::mdII(WY_BASE_URL, false).toUtf8());
    request->setRawHeader("User-Agent", TTK::Algorithm::mdII(WY_UA_URL, MDII_UA_KEY, false).toUtf8());
    request->setRawHeader("Cookie", QString("MUSIC_U=%1; NMTID=%2; ").arg(TTK::Algorithm::mdII(WY_COOKIE_URL, false), TTK::Algorithm::mdII(WY_NMTID_URL, MDII_UA_KEY, false)).toUtf8());
    TTK::setSslConfiguration(request);
    TTK::makeContentTypeHeader(request);
}

QString ReqWYInterface::makeSongArtist(const QString &in, const QString &name)
{
    const QString &artistName = TTK::String::charactersReplace(name);
    return in.isEmpty() ? artistName : (in + ";" + artistName);
}

QString ReqWYInterface::makeCoverPixmapUrl(const QString &url)
{
    return url + TTK::Algorithm::mdII("dCt3T2JSbmJ2LzFuOUZBalAwTnUvNkRpc3dZPQ==", false);
}

QByteArray ReqWYInterface::makeTokenRequest(QNetworkRequest *request, const QString &query, const QString &data)
{
    QAlgorithm::Aes aes;
    QByteArray param = aes.encryptCBC(data.toUtf8(), "0CoJUm6Qyw8W8jud", "0102030405060708");
    param = aes.encryptCBC(param, "a44e542eaac91dce", "0102030405060708");
    TTK::Url::urlEncode(param);

    request->setUrl(query);
    ReqWYInterface::makeRequestRawHeader(request);
    return "params=" + param + "&encSecKey=" + WY_SECKRY_STRING;
}

QByteArray ReqWYInterface::makeTokenRequest(QNetworkRequest *request, const QString &url, const QString &query, const QString &data)
{
    const QString &message = "nobody" + query + "use" + data + "md5forencrypt";
    const QByteArray &digest = QCryptographicHash::hash(message.toUtf8(), QCryptographicHash::Md5).toHex();
    const QString &body = query + "-36cd479b6b5-" + data + "-36cd479b6b5-" + digest;

    QAlgorithm::Aes aes;
    QByteArray param = aes.encryptECB(body.toUtf8(), "e82ckenh8dichen8", true);

    request->setUrl(url);
    ReqWYInterface::makeRequestRawHeader(request);
    return "params=" + param.toUpper();
}

static void parseSongPropertyV1(TTK::MusicSongInformation *info, int bitrate)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == bitrate)
        {
            return;
        }
    }

    TTK_INFO_STREAM("parse song property in v1 module");

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(WY_SONG_PATH_V1_URL, false).arg(bitrate * 1000).arg(info->m_songId));
    ReqWYInterface::makeRequestRawHeader(&request);

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

            const int rate = value["br"].toInt() / 1000;
            if(rate == bitrate || (bitrate > TTK_BN_500 && rate > TTK_BN_500))
            {
                TTK::MusicSongProperty prop;
                prop.m_url = value["url"].toString();
                prop.m_bitrate = bitrate;
                prop.m_size = TTK::Number::sizeByteToLabel(value["size"].toInt());
                prop.m_format = value["type"].toString();
                info->m_songProps.append(prop);
            }
        }
    }
}

static void parseSongPropertyV2(TTK::MusicSongInformation *info, int bitrate)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == bitrate)
        {
            return;
        }
    }

    TTK_INFO_STREAM("parse song property in v2 module");

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_SONG_PATH_V2_URL, false),
                      TTK::Algorithm::mdII(WY_SONG_PATH_V2_DATA_URL, false).arg(info->m_songId).arg(bitrate * 1000));

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

                const int rate = value["br"].toInt() / 1000;
                if(rate == bitrate || (bitrate > TTK_BN_500 && rate > TTK_BN_500))
                {
                    TTK::MusicSongProperty prop;
                    prop.m_url = value["url"].toString();
                    prop.m_bitrate = bitrate;
                    prop.m_size = TTK::Number::sizeByteToLabel(value["size"].toInt());
                    prop.m_format = value["type"].toString();
                    info->m_songProps.append(prop);
                }
            }
        }
    }
}

static void parseSongPropertyV3(TTK::MusicSongInformation *info, int bitrate)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == bitrate)
        {
            return;
        }
    }

    TTK_INFO_STREAM("parse song property in v3 module");

    QString format;
    switch(bitrate)
    {
        case TTK_BN_128: format = TTK::Algorithm::mdII(_128KBPS, false); break;
        case TTK_BN_320: format = TTK::Algorithm::mdII(_320KBPS, false); break;
        case TTK_BN_1000: format = TTK::Algorithm::mdII(_999KBPS, false); break;
        default: return;
    }

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_SONG_PATH_V3_URL, false),
                      TTK::Algorithm::mdII(WY_SONG_PATH_V3_QUERY_URL, false),
                      TTK::Algorithm::mdII(WY_SONG_PATH_V3_DATA_URL, false).arg(info->m_songId, format));

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

                const int rate = value["br"].toInt() / 1000;
                if(rate == bitrate || (bitrate > TTK_BN_500 && rate > TTK_BN_500))
                {
                    TTK::MusicSongProperty prop;
                    prop.m_url = value["url"].toString();
                    prop.m_bitrate = bitrate;
                    prop.m_size = TTK::Number::sizeByteToLabel(value["size"].toInt());
                    prop.m_format = value["type"].toString();
                    info->m_songProps.append(prop);
                }
            }
        }
    }
}

static void parseSongProperty(TTK::MusicSongInformation *info, int bitrate)
{
    parseSongPropertyV1(info, bitrate);
    parseSongPropertyV2(info, bitrate);
    parseSongPropertyV3(info, bitrate);
}

void ReqWYInterface::parseFromSongProperty(TTK::MusicSongInformation *info, int bitrate)
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    if(info->m_formatProps.isEmpty())
    {
        parseSongProperty(info, TTK_BN_128);
        return;
    }

    if(bitrate == TTK_BN_0)
    {
        const int maxBr = info->m_formatProps.toInt();
        if(maxBr == TTK_BN_1000)
        {
            parseSongProperty(info, TTK_BN_128);
            parseSongProperty(info, TTK_BN_192);
            parseSongProperty(info, TTK_BN_320);
            parseSongProperty(info, TTK_BN_1000);
        }
        else if(maxBr == TTK_BN_320)
        {
            parseSongProperty(info, TTK_BN_128);
            parseSongProperty(info, TTK_BN_192);
            parseSongProperty(info, TTK_BN_320);
        }
        else if(maxBr == TTK_BN_192)
        {
            parseSongProperty(info, TTK_BN_128);
            parseSongProperty(info, TTK_BN_192);
        }
        else
        {
            parseSongProperty(info, TTK_BN_128);
        }
    }
    else
    {
        parseSongProperty(info, bitrate);
    }
}

void ReqWYInterface::parseFromSongProperty(TTK::MusicSongInformation *info, const QVariantMap &key)
{
    int maxBr = TTK_BN_1000;
    const QVariantMap &privilege = key["privilege"].toMap();
    if(!privilege.isEmpty())
    {
        const QString &brStr = privilege["maxbr"].toString();
        if(brStr == "999000")
        {
            maxBr = TTK_BN_1000;
        }
        else if(brStr == "320000" || brStr == "192000" || brStr == "190000")
        {
            maxBr = TTK_BN_320;
        }
        else if(brStr == "160000")
        {
            maxBr = TTK_BN_192;
        }
        else
        {
            maxBr = TTK_BN_128;
        }
    }

    info->m_formatProps = QString::number(maxBr);
}
