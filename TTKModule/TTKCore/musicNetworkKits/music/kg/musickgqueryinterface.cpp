#include "musickgqueryinterface.h"
#include "musicabstractqueryrequest.h"

void ReqKGInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    request->setRawHeader("User-Agent", TTK::Algorithm::mdII(KG_UA_URL, ALG_UA_KEY, false).toUtf8());
    TTK::setSslConfiguration(request);
    TTK::makeContentTypeHeader(request);
}

void ReqKGInterface::parseFromSongAlbumLrc(TTK::MusicSongInformation *info)
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_SONG_INFO_URL, false).arg(info->m_songId));
    ReqKGInterface::makeRequestRawHeader(&request);

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
        if(value["errcode"].toInt() == 0 && value.contains("data"))
        {
            value = value["data"].toMap();
            info->m_songName = TTK::String::charactersReplace(value["songname"].toString());
            info->m_artistId = value["singerid"].toString();
            info->m_artistName = TTK::String::charactersReplace(value["singername"].toString());
            info->m_coverUrl = value["imgurl"].toString().replace("{size}", "480");
            info->m_lrcUrl = TTK::Algorithm::mdII(KG_SONG_LRC_URL, false).arg(value["songname"].toString(), info->m_songId).arg(value["duration"].toInt() * TTK_DN_S2MS);
        }
    }
}

void ReqKGInterface::parseFromSongAlbumInfo(TTK::MusicSongInformation *info, const QString &album)
{
    if(album.isEmpty())
    {
        return;
    }

    MusicResultDataItem result;
    parseFromSongAlbumInfo(&result, info->m_songId, album);

    info->m_albumId = result.m_id;
    info->m_albumName = result.m_name;
}

void ReqKGInterface::parseFromSongAlbumInfo(MusicResultDataItem *info, const QString &hash, const QString &album)
{
    if(hash.isEmpty() || album.isEmpty())
    {
        return;
    }

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_ALBUM_INFO_URL, false).arg(hash, album));
    ReqKGInterface::makeRequestRawHeader(&request);

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
        if(value["error_code"].toInt() == 0 && value.contains("data"))
        {
            QVariantList datas = value["data"].toList();
            for(const QVariant &var : qAsConst(datas))
            {
                if(var.isNull())
                {
                    continue;
                }

                for(const QVariant &al : var.toList())
                {
                    if(al.isNull())
                    {
                        continue;
                    }

                    value = al.toMap();
                    info->m_id = value["album_id"].toString();
                    info->m_name = TTK::String::charactersReplace(value["album_name"].toString());
                    info->m_description = value["publish_company"].toString();
                    info->m_updateTime = value["publish_date"].toString();
                    info->m_category = value["language"].toString();
                    break;
                }
            }
        }
    }
}

static void parseSongProperty(TTK::MusicSongInformation *info, const QString &hash, int bitrate)
{
    if(hash.isEmpty())
    {
        return;
    }

    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == bitrate)
        {
            return;
        }
    }

    bool foundProp = false;
    {
        const QByteArray &encodedData = TTK::Algorithm::md5(QString("%1kgcloudv2").arg(hash).toUtf8());

        QNetworkRequest request;
        request.setUrl(TTK::Algorithm::mdII(KG_SONG_DETAIL_URL, false).arg(hash, encodedData.constData()));
        ReqKGInterface::makeRequestRawHeader(&request);

        const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
        if(!bytes.isEmpty())
        {
            QJson::Parser json;
            bool ok = false;
            const QVariant &data = json.parse(bytes, &ok);
            if(ok)
            {
                const QVariantMap &value = data.toMap();
                if(value.contains("status") && value["status"].toInt() == 1)
                {
                    TTK::MusicSongProperty prop;
                    prop.m_url = value["url"].toString();
                    prop.m_size = TTK::Number::sizeByteToLabel(value["fileSize"].toInt());
                    prop.m_format = value["extName"].toString();
                    prop.m_bitrate = TTK::Number::bitrateToNormal(bitrate);
                    info->m_songProps.append(prop);
                    foundProp = true;
                }
            }
        }
    }

    if(!foundProp)
    {
        const QString &mid = TTK::Algorithm::mdII("Wk51dktMOHJXUTdmM1VsVUVXTFM5RTlYQ05laDE0Z2lZMzFPL1M1VUJSaHd1N0kwRDQxdkpWVFJPZTQ9", false);
        const QString &sign = TTK::Algorithm::mdII("SVhlNmFTaWpqdVhYVTAwaHh4QllwRkFGSmJpY0VSZUhXQmQrV2Q4WHo0eXVCWm1zK1p0RkVRPT0=", false);
        const QString &user = "0";
        const QByteArray &encodedData = TTK::Algorithm::md5((hash + sign + mid + user).toUtf8());

        QNetworkRequest request;
        request.setUrl(TTK::Algorithm::mdII(KG_SONG_DETAIL_OLD_URL, false).arg(mid, hash, user, encodedData.constData()));
        request.setRawHeader("x-router", TTK::Algorithm::mdII("MTJnUGtpL0hqWXhZQmlCNE9hVzVyREF0QXZmeVBNNVc=", false).toUtf8());
        ReqKGInterface::makeRequestRawHeader(&request);

        const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
        if(!bytes.isEmpty())
        {
            QJson::Parser json;
            bool ok = false;
            const QVariant &data = json.parse(bytes, &ok);
            if(ok)
            {
                const QVariantMap &value = data.toMap();
                if(value.contains("status") && value["status"].toInt() == 1)
                {
                    TTK::MusicSongProperty prop;
                    prop.m_size = TTK::Number::sizeByteToLabel(value["fileSize"].toInt());
                    prop.m_format = value["extName"].toString();
                    prop.m_bitrate = TTK::Number::bitrateToNormal(bitrate);

                    const QVariantList &datas = value["url"].toList();
                    if(!datas.isEmpty())
                    {
                        prop.m_url = datas.front().toString();
                        info->m_songProps.append(prop);
                    }
                }
            }
        }
    }
}


void ReqKGInterface::parseFromSongProperty(TTK::MusicSongInformation *info, int bitrate)
{
    const QStringList &formats = info->m_formatProps.split("|");
    if(info->m_formatProps.isEmpty() || formats.count() < 4)
    {
        parseSongProperty(info, info->m_songId, TTK_BN_128);
        return;
    }

    if(bitrate == TTK_BN_0)
    {
        parseSongProperty(info, formats[0], TTK_BN_128);
        parseSongProperty(info, formats[1], TTK_BN_192);
        parseSongProperty(info, formats[2], TTK_BN_320);
        parseSongProperty(info, formats[3], TTK_BN_1000);
    }
    else
    {
        int index = 0;
        switch(bitrate)
        {
            case TTK_BN_128: index = 0; break;
            case TTK_BN_192: index = 1; break;
            case TTK_BN_320: index = 2; break;
            case TTK_BN_1000: index = 3; break;
        }

        parseSongProperty(info, formats[index], bitrate);
    }
}

void ReqKGInterface::parseFromSongProperty(TTK::MusicSongInformation *info, const QVariantMap &key)
{
    info->m_formatProps = key["hash"].toString() + "|" + key["128hash"].toString() + "|" + key["320hash"].toString() + "|" + key["sqhash"].toString();
}
