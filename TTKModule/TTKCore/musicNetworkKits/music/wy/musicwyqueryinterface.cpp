#include "musicwyqueryinterface.h"
#include "musicurlutils.h"
#include "musicabstractnetwork.h"

#include "qalgorithm/aeswrapper.h"

static constexpr const char *BN_128KBPS = "QXMyZkZJc2dIb1FOenJlTg==";
static constexpr const char *BN_320KBPS = "UThNR09kcDRXNG9qbG45Ng==";
static constexpr const char *BN_999KBPS = "VGF0djlKc01mL1QxM1pyNQ==";

static constexpr const char *WY_UA_URL = "Vkloais0Z0Q4Smp1aElYSXFUVy96SXdoZ2VsTURzdDBpZEZjT21jMy9PRE1DMzdwQ0p5MllueTkrRGdzb2VkdlJMemJhWEFReHlUUk9uSEtMTHRLUzRxS3psczJiRW9LZ3BQbHh1Z3lGRlhEQk5JbVVGb1NDN1JzTDhZMjBybllLcndKSHlSNC94QzRJWXlL";
static constexpr const char *WY_COOKIE_URL = "eU5NVy9LRm5jWE9pdUU1MzNQNi9OcjVUbi9vSGhYcGVaQWc0NjlPbCt1WnNDbDlONDg1SSthTi9JUDQ1YVpsRg==";
static constexpr const char *WY_USER_URL = "bSs4UEJOUzFwRWpzRVY0WUVJekhzU0Nzc1JZT0pLc2tOdFNaM05DWnRhM3JHT0kzSWpQb1MyNXJkK3A5YmV3YklpM082dDk1amVjejV0NlV5aHYybkF4Zjd3L2NSblJkYmRXOS8xVlJOWnYvMWhweUxsSGhtSGVPYnFQZE10d01sNWswZFNzYnVEOGhBWkpHdHFTUGpJNVROdWM9";
static constexpr const char *WY_RAND_URL = "V0Zsb2tGREU1R0thR29GYjRYdk5jbm5NZjEvN1d6WWNiQlZqb2k4eE1nTm9lTHlBRktkQTBOaXNjQ3ZHU0ZMSw==";
static constexpr const char *WY_BASE_URL = "MVNVTXo4bW9WdHhXR1dXeTZmU3k5dmFOcGlua1VOMlE=";
static constexpr const char *WY_SECKRY_URL = "411571dca16717d9af5ef1ac97a8d21cb740329890560688b1b624de43f49fdd7702493835141b06ae45f1326e264c98c24ce87199c1a776315e5f25c11056b02dd92791fcc012bff8dd4fc86e37888d5ccc060f7837b836607dbb28bddc703308a0ba67c24c6420dd08eec2b8111067486c907b6e53c027ae1e56c188bc568e";

void ReqWYInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    request->setRawHeader("Referer", TTK::Algorithm::mdII(WY_BASE_URL, false).toUtf8());
    request->setRawHeader("Origin", TTK::Algorithm::mdII(WY_BASE_URL, false).toUtf8());
    request->setRawHeader("User-Agent", TTK::Algorithm::mdII(WY_UA_URL, MDII_UA_KEY, false).toUtf8());
    request->setRawHeader("Cookie", TTK::Algorithm::mdII(WY_COOKIE_URL, false).arg(TTK::Algorithm::mdII(WY_USER_URL, false), TTK::Algorithm::mdII(WY_RAND_URL, MDII_UA_KEY, false)).toUtf8());
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
    return "params=" + param + "&encSecKey=" + WY_SECKRY_URL;
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
        case TTK_BN_128: format = TTK::Algorithm::mdII(BN_128KBPS, false); break;
        case TTK_BN_320: format = TTK::Algorithm::mdII(BN_320KBPS, false); break;
        case TTK_BN_1000: format = TTK::Algorithm::mdII(BN_999KBPS, false); break;
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
