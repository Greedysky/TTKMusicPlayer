#include "musickgqueryinterface.h"
#include "musicabstractqueryrequest.h"

void MusicKGInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    request->setRawHeader("User-Agent", TTK::Algorithm::mdII(KG_UA_URL, ALG_UA_KEY, false).toUtf8());
    TTK::setSslConfiguration(request);
    TTK::makeContentTypeHeader(request);
}

void MusicKGInterface::parseFromSongProperty(TTK::MusicSongInformation *info, const QString &hash)
{
    if(hash.isEmpty())
    {
        return;
    }

    const QByteArray &encodedData = TTK::Algorithm::md5(QString("%1kgcloudv2").arg(hash).toUtf8());

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_SONG_DETAIL_URL, false).arg(hash, encodedData.constData()));
    MusicKGInterface::makeRequestRawHeader(&request);

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
        const QVariantMap &value = data.toMap();
        if(!value.isEmpty() && !value.contains("error"))
        {
            const int bitrate = value["bitRate"].toInt();
            if(bitrate == 0)
            {
                return;
            }

            TTK::MusicSongProperty prop;
            prop.m_url = value["url"].toString();
            prop.m_size = TTK::Number::sizeByteToLabel(value["fileSize"].toInt());
            prop.m_format = value["extName"].toString();
            prop.m_bitrate = TTK::Number::bitrateToNormal(bitrate / 1000);
            info->m_songProps.append(prop);
        }
    }
}

void MusicKGInterface::parseFromSongProperty(TTK::MusicSongInformation *info, const QVariantMap &key, bool all)
{
    if(all)
    {
        parseFromSongProperty(info, key["hash"].toString());
        parseFromSongProperty(info, key["128hash"].toString());
        parseFromSongProperty(info, key["320hash"].toString());
        parseFromSongProperty(info, key["sqhash"].toString());
    }
    else
    {
        parseFromSongProperty(info, key["hash"].toString());
        parseFromSongProperty(info, key["128hash"].toString());
    }
}

void MusicKGInterface::parseFromSongLrcAndPicture(TTK::MusicSongInformation *info)
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_SONG_INFO_URL, false).arg(info->m_songId));
    MusicKGInterface::makeRequestRawHeader(&request);

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
            info->m_artistId = value["singerid"].toString();
            info->m_coverUrl = value["imgurl"].toString().replace("{size}", "480");
            info->m_lrcUrl = TTK::Algorithm::mdII(KG_SONG_LRC_URL, false).arg(value["songname"].toString(), info->m_songId).arg(value["duration"].toInt() * TTK_DN_S2MS);
        }
    }
}

void MusicKGInterface::parseFromSongAlbumInfo(MusicResultDataItem *info, const QString &album)
{
    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_ALBUM_INFO_URL, false).arg(album));
    MusicKGInterface::makeRequestRawHeader(&request);

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
        if(value.contains("data"))
        {
            value = value["data"].toMap();
            info->m_nickName = value["albumname"].toString();
            info->m_description = info->m_nickName + TTK_SPLITER +
                                  value["language"].toString() + TTK_SPLITER +
                                  value["company"].toString() + TTK_SPLITER +
                                  value["publishtime"].toString().left(10);
//            info->m_coverUrl = value["imgurl"].toString();
        }
    }
}
