#include "musicwyqueryinterface.h"
#include "musicurlutils.h"
#include "musicabstractnetwork.h"

#include "qalgorithm/aeswrapper.h"

void ReqWYInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    request->setRawHeader("Referer", TTK::Algorithm::mdII(WY_BASE_URL, false).toUtf8());
    request->setRawHeader("Origin", TTK::Algorithm::mdII(WY_BASE_URL, false).toUtf8());
    request->setRawHeader("User-Agent", TTK::Algorithm::mdII(WY_UA_URL, ALG_UA_KEY, false).toUtf8());
    request->setRawHeader("Cookie", QString("MUSIC_U=%1; NMTID=%2; ").arg(TTK::Algorithm::mdII(WY_COOKIE_URL, false), TTK::Algorithm::mdII(WY_NMTID_URL, ALG_UA_KEY, false)).toUtf8());
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

QByteArray ReqWYInterface::makeTokenRequest(QNetworkRequest *request, const QString &query, const QString &type)
{
    QAlgorithm::Aes aes;
    QByteArray parameter = aes.encryptCBC(type.toUtf8(), "0CoJUm6Qyw8W8jud", "0102030405060708");
    parameter = aes.encryptCBC(parameter, "a44e542eaac91dce", "0102030405060708");
    TTK::Url::urlEncode(parameter);

    request->setUrl(query);
    ReqWYInterface::makeRequestRawHeader(request);

    return "params=" + parameter + "&encSecKey=" + WY_SECKRY_STRING;
}

static void parseSongPropertyOld(TTK::MusicSongInformation *info, int bitrate)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == bitrate)
        {
            return;
        }
    }

    TTK_INFO_STREAM("parse song property in old module");

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(WY_SONG_INFO_OLD_URL, false).arg(bitrate * 1000).arg(info->m_songId));
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

static void parseSongPropertyNew(TTK::MusicSongInformation *info, int bitrate)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == bitrate)
        {
            return;
        }
    }

    TTK_INFO_STREAM("parse song property in new module");

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
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

static void parseSongPropertyCGG(TTK::MusicSongInformation *info, int bitrate)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == bitrate)
        {
            return;
        }
    }

    TTK_INFO_STREAM("parse song property in cgg module");

    TTK::MusicSongProperty prop;
    prop.m_bitrate = bitrate;
    prop.m_size = TTK::Number::sizeByteToLabel(info->m_duration, bitrate);

    QString format;
    if(bitrate == TTK_BN_128)
    {
        format = TTK::Algorithm::mdII("QXMyZkZJc2dIb1FOenJlTg==", false);
        prop.m_format = MP3_FILE_SUFFIX;
    }
    else if(bitrate == TTK_BN_320)
    {
        format = TTK::Algorithm::mdII("UThNR09kcDRXNG9qbG45Ng==", false);
        prop.m_format = MP3_FILE_SUFFIX;
    }
    else if(bitrate == TTK_BN_1000)
    {
        format = TTK::Algorithm::mdII("VGF0djlKc01mL1QxM1pyNQ==", false);
        prop.m_format = FLAC_FILE_SUFFIX;
    }
    else
    {
        return;
    }

    prop.m_url = TTK::Algorithm::mdII(WY_SONG_DETAIL_CGG_URL, false).arg(info->m_songId, format);
    info->m_songProps.append(prop);
}

static void parseSongProperty(TTK::MusicSongInformation *info, int bitrate)
{
    parseSongPropertyOld(info, bitrate);
    parseSongPropertyNew(info, bitrate);
    parseSongPropertyCGG(info, bitrate);
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
