#include "musickgqueryinterface.h"
#include "musicunityqueryinterface.h"

static constexpr const char *KG_UA_URL = "cGhYNDZVdmNaVG5KZk50NVFvcUJyYWVQdmdNTkFTMmM=";
static constexpr const char *KG_COOKIE_URL = "M1BlYjdvWUlwdHJIYXgzeGpUVDlWbnB6ZGZPaUxWVzhKdEZ2RHBHR1RBckFUbG82ZzlOOHRnMW0wUUZpVmdDNGNQMFFjTXd1OVhidmwvdHh6NjNkZ08reXEzWUl0ZXl3d21Ka3ErM1Q1eEp3WUs0YkYvV3dWVUFDSVd3YmI3c3U3c09pZTlIM1FQRFUyMG43Z2ZqRXJVTHhubkhjRC9EN3hVbzhia1QzdUZQU3orRzZZRmRncGZLazhkMD0=";

void ReqKGInterface::makeRequestRawHeader(QNetworkRequest *request) noexcept
{
    request->setRawHeader("User-Agent", TTK::Algorithm::mdII(KG_UA_URL, MDII_UA_KEY, false).toUtf8());
    request->setRawHeader("Cookie", TTK::Algorithm::mdII(KG_COOKIE_URL, false).toUtf8());

    TTK::setSslConfiguration(request);
    TTK::makeContentTypeHeader(request);
}

QString ReqKGInterface::makeSongArtist(const QString &name)
{
    return TTK::String::charactersReplace(name).replace("+", ";");
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

    QJsonParseError ok;
    const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
    if(QJsonParseError::NoError == ok.error)
    {
        QVariantMap value = json.toVariant().toMap();
        if(value["errcode"].toInt() == 0 && value.contains("data"))
        {
            value = value["data"].toMap();
            info->m_songName = TTK::String::charactersReplace(value["songname"].toString());
            info->m_artistId = value["singerid"].toString();
            info->m_artistName = ReqKGInterface::makeSongArtist(value["singername"].toString());
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

    MusicResultDataItem item;
    parseFromSongAlbumInfo(&item, info->m_songId, album);

    info->m_albumId = item.m_id;
    info->m_albumName = item.m_name;
}

void ReqKGInterface::parseFromSongAlbumInfo(MusicResultDataItem *item, const QString &hash, const QString &album)
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

    QJsonParseError ok;
    const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
    if(QJsonParseError::NoError == ok.error)
    {
        QVariantMap value = json.toVariant().toMap();
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
                    item->m_id = value["album_id"].toString();
                    item->m_name = TTK::String::charactersReplace(value["album_name"].toString());
                    item->m_description = value["publish_company"].toString();
                    item->m_time = value["publish_date"].toString();
                    item->m_category = value["language"].toString();
                    break;
                }
            }
        }
    }
}

static void parseSongPropertyV1(TTK::MusicSongInformation *info, const QString &hash, int bitrate)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == bitrate)
        {
            return;
        }
    }

    TTK_INFO_STREAM("parse song" << bitrate << "kbps property in v1 module");

    const QByteArray &key = TTK::Algorithm::md5(QString("%1kgcloudv2").arg(hash).toUtf8());

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_SONG_PATH_V1_URL, false).arg(hash, key.constData()));
    ReqKGInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
    if(!bytes.isEmpty())
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            const QVariantMap &value = json.toVariant().toMap();
            if(value.contains("status") && value["status"].toInt() == 1)
            {
                TTK::MusicSongProperty prop;
                prop.m_url = value["url"].toString();
                prop.m_size = TTK::Number::sizeByteToLabel(value["fileSize"].toInt());
                prop.m_format = value["extName"].toString();
                prop.m_bitrate = bitrate;
                info->m_songProps.append(prop);
            }
        }
    }
}

static void parseSongPropertyV2(TTK::MusicSongInformation *info, const QString &hash, int bitrate)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == bitrate)
        {
            return;
        }
    }

    TTK_INFO_STREAM("parse song" << bitrate << "kbps property in v2 module");

    const QString &mid = TTK::Algorithm::mdII("Wk51dktMOHJXUTdmM1VsVUVXTFM5RTlYQ05laDE0Z2lZMzFPL1M1VUJSaHd1N0kwRDQxdkpWVFJPZTQ9", false);
    const QString &sign = TTK::Algorithm::mdII("SVhlNmFTaWpqdVhYVTAwaHh4QllwRkFGSmJpY0VSZUhXQmQrV2Q4WHo0eXVCWm1zK1p0RkVRPT0=", false);
    const QString &user = "0";
    const QByteArray &key = TTK::Algorithm::md5((hash + sign + mid + user).toUtf8());

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_SONG_PATH_V2_URL, false).arg(mid, hash, user, key.constData()));
    request.setRawHeader("x-router", TTK::Algorithm::mdII("MTJnUGtpL0hqWXhZQmlCNE9hVzVyREF0QXZmeVBNNVc=", false).toUtf8());
    ReqKGInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
    if(!bytes.isEmpty())
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            const QVariantMap &value = json.toVariant().toMap();
            if(value.contains("status") && value["status"].toInt() == 1)
            {
                const QVariantList &datas = value["url"].toList();
                if(!datas.isEmpty())
                {
                    TTK::MusicSongProperty prop;
                    prop.m_url = datas.front().toString();
                    prop.m_size = TTK::Number::sizeByteToLabel(value["fileSize"].toInt());
                    prop.m_format = value["extName"].toString();
                    prop.m_bitrate = bitrate;
                    info->m_songProps.append(prop);
                }
            }
        }
    }
}

static void parseSongPropertyV3(TTK::MusicSongInformation *info, const QString &module, QString &id, int bitrate)
{
    const qint64 time = TTKDateTime::currentTimestamp();
    const QByteArray &key = TTK::Algorithm::md5(TTK::Algorithm::mdII(KG_SONG_PATH_V3_DATA_URL, false).arg(time).arg(module).toUtf8());

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_SONG_PATH_V3_URL, false).arg(time).arg(module, key.constData()));
    ReqKGInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
    if(!bytes.isEmpty())
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value.contains("data") && value["err_code"].toInt() == 0)
            {
                value = value["data"].toMap();
                id = value["encode_album_audio_id"].toString();

                const int rate = value["bitrate"].toInt();
                const QString &url = value["play_url"].toString();

                if(rate == bitrate && !url.isEmpty())
                {
                    TTK::MusicSongProperty prop;
                    prop.m_url = url;
                    prop.m_size = TTK::Number::sizeByteToLabel(value["filesize"].toInt());
                    prop.m_format = bitrate > TTK_BN_320 ? FLAC_FILE_SUFFIX : MP3_FILE_SUFFIX;;
                    prop.m_bitrate = bitrate;
                    info->m_songProps.append(prop);
                }
            }
        }
    }
}

static void parseSongPropertyV3(TTK::MusicSongInformation *info, const QString &hash, int bitrate)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == bitrate)
        {
            return;
        }
    }

    TTK_INFO_STREAM("parse song" << bitrate << "kbps property in v3 module");

    QString id;
    parseSongPropertyV3(info, "hash=" + hash, id, bitrate);

    if(!id.isEmpty())
    {
        parseSongPropertyV3(info, "encode_album_audio_id=" + id, id, bitrate);
    }
}

static void parseSongPropertyV4(TTK::MusicSongInformation *info, const QString &hash, int bitrate)
{
    for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
    {
        if(prop.m_bitrate == bitrate)
        {
            return;
        }
    }

    TTK_INFO_STREAM("parse song" << bitrate << "kbps property in v4 module");

    if(bitrate == TTK_BN_128 || bitrate == TTK_BN_320 || bitrate == TTK_BN_1000)
    {
        ReqUnityInterface::parseFromSongProperty(info, QUERY_KG_INTERFACE, hash, bitrate);
    }
}

static void parseSongProperty(TTK::MusicSongInformation *info, const QString &hash, int bitrate)
{
    if(hash.isEmpty())
    {
        return;
    }

    parseSongPropertyV1(info, hash, bitrate);
    parseSongPropertyV2(info, hash, bitrate);
    parseSongPropertyV3(info, hash, bitrate);
    parseSongPropertyV4(info, hash, bitrate);
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
