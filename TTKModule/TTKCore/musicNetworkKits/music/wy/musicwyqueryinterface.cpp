#include "musicwyqueryinterface.h"
#include "musicurlutils.h"
#include "musicsettingmanager.h"
#include "musicabstractnetwork.h"

#include "qalgorithm/aeswrapper.h"

void MusicWYInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    request->setRawHeader("Referer", MusicUtils::Algorithm::mdII(WY_BASE_URL, false).toUtf8());
    request->setRawHeader("Origin", MusicUtils::Algorithm::mdII(WY_BASE_URL, false).toUtf8());
    request->setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(WY_UA_URL, ALG_UA_KEY, false).toUtf8());
    request->setRawHeader("Cookie", QString("MUSIC_U=%1; NMTID=%2; ").arg(MusicUtils::Algorithm::mdII(WY_COOKIE_URL, false),
                                            MusicUtils::Algorithm::mdII(WY_NMTID_URL, ALG_UA_KEY, false)).toUtf8());
    MusicObject::setSslConfiguration(request);
}


QByteArray MusicWYQueryInterface::makeTokenQueryUrl(QNetworkRequest *request, const QString &query, const QString &type) const
{
    QAlgorithm::Aes aes;
    QByteArray parameter = aes.encryptCBC(type.toUtf8(), "0CoJUm6Qyw8W8jud", "0102030405060708");
    parameter = aes.encryptCBC(parameter, "a44e542eaac91dce", "0102030405060708");
    MusicUtils::Url::urlEncode(parameter);

    request->setUrl(query);
    MusicWYInterface::makeRequestRawHeader(request);

    return "params=" + parameter + "&encSecKey=" + WY_SECKRY_STRING.toUtf8();
}

void MusicWYQueryInterface::readFromMusicSongProperty(MusicObject::MusicSongInformation *info, int bitrate) const
{
    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(WY_SONG_INFO_OLD_URL, false).arg(bitrate * 1000).arg(info->m_songId));
    MusicWYInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = MusicObject::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    QJson::Parser json;
    bool ok;
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

            MusicObject::MusicSongProperty prop;
            prop.m_url = value["url"].toString();
            prop.m_bitrate = bitrate;

            if(prop.m_url.isEmpty())
            {
                readFromMusicSongPropertyNew(info, bitrate);
                return;
            }

            if(info->m_songProps.contains(prop))
            {
                return;
            }

            prop.m_size = MusicUtils::Number::sizeByte2Label(value["size"].toInt());
            prop.m_format = value["type"].toString();
            info->m_songProps.append(prop);
        }
    }
}

void MusicWYQueryInterface::readFromMusicSongProperty(MusicObject::MusicSongInformation *info, const QVariantMap &key, const QString &quality, bool all) const
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
            readFromMusicSongProperty(info, MB_128);
            readFromMusicSongProperty(info, MB_192);
            readFromMusicSongProperty(info, MB_320);
            readFromMusicSongProperty(info, MB_1000);
        }
        else if(maxBr == MB_320)
        {
            readFromMusicSongProperty(info, MB_128);
            readFromMusicSongProperty(info, MB_192);
            readFromMusicSongProperty(info, MB_320);
        }
        else if(maxBr == MB_192)
        {
            readFromMusicSongProperty(info, MB_128);
            readFromMusicSongProperty(info, MB_192);
        }
        else
        {
            readFromMusicSongProperty(info, MB_128);
        }
    }
    else
    {
        if(quality == QObject::tr("SD") && maxBr >= MB_128)
        {
            readFromMusicSongProperty(info, MB_128);
        }
        else if(quality == QObject::tr("HQ") && maxBr >= MB_192)
        {
            readFromMusicSongProperty(info, MB_192);
        }
        else if(quality == QObject::tr("SQ") && maxBr >= MB_320)
        {
            readFromMusicSongProperty(info, MB_320);
        }
        else if(quality == QObject::tr("CD") && maxBr >= MB_1000)
        {
            readFromMusicSongProperty(info, MB_1000);
        }
    }
}

void MusicWYQueryInterface::readFromMusicSongPropertyNew(MusicObject::MusicSongInformation *info, int bitrate) const
{
    QNetworkRequest request;
    const QByteArray &parameter = makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(WY_SONG_PATH_URL, false),
                      MusicUtils::Algorithm::mdII(WY_SONG_PATH_DATA_URL, false).arg(info->m_songId).arg(bitrate * 1000));

    const QByteArray &bytes = MusicObject::syncNetworkQueryForPost(&request, parameter);
    if(bytes.isEmpty())
    {
        return;
    }

    QJson::Parser json;
    bool ok;
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
                MusicObject::MusicSongProperty prop;
                prop.m_url = value["url"].toString();
                prop.m_bitrate = bitrate;
                if(prop.m_url.isEmpty() || info->m_songProps.contains(prop))
                {
                    break;
                }

                prop.m_size = MusicUtils::Number::sizeByte2Label(value["size"].toInt());
                prop.m_format = value["type"].toString();
                info->m_songProps.append(prop);
            }
        }
    }
}

void MusicWYQueryInterface::readFromMusicSongPropertyNew(MusicObject::MusicSongInformation *info, const QVariantMap &key, const QString &quality, bool all) const
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
            readFromMusicSongProperty(info, MB_128);
            readFromMusicSongProperty(info, MB_192);
            readFromMusicSongProperty(info, MB_320);
            readFromMusicSongProperty(info, MB_1000);
        }
        else if(maxBr == MB_320)
        {
            readFromMusicSongProperty(info, MB_128);
            readFromMusicSongProperty(info, MB_192);
            readFromMusicSongProperty(info, MB_320);
        }
        else if(maxBr == MB_192)
        {
            readFromMusicSongProperty(info, MB_128);
            readFromMusicSongProperty(info, MB_192);
        }
        else
        {
            readFromMusicSongProperty(info, MB_128);
        }
    }
    else
    {
        if(quality == QObject::tr("SD") && maxBr >= MB_128)
        {
            readFromMusicSongProperty(info, MB_128);
        }
        else if(quality == QObject::tr("HQ") && maxBr >= MB_192)
        {
            readFromMusicSongProperty(info, MB_192);
        }
        else if(quality == QObject::tr("SQ") && maxBr >= MB_320)
        {
            readFromMusicSongProperty(info, MB_320);
        }
        else if(quality == QObject::tr("CD") && maxBr >= MB_1000)
        {
            readFromMusicSongProperty(info, MB_1000);
        }
    }
}
