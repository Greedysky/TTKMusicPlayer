#include "musickgqueryartistrequest.h"

MusicKGQueryArtistRequest::MusicKGQueryArtistRequest(QObject *parent)
    : MusicQueryArtistRequest(parent)
{
    m_queryServer = QUERY_KG_INTERFACE;
}

void MusicKGQueryArtistRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(className() << "startToSearch" << value);

    deleteAll();
    m_queryValue = value;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_ARTIST_URL, false).arg(value).arg(0).arg(50));
    MusicKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGQueryArtistRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(className() << "startToQueryResult" << info->m_songId << bitrate << "kbps");
    MusicPageQueryRequest::downLoadFinished();

    TTK_NETWORK_QUERY_CHECK();
    MusicKGInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    MusicQueryArtistRequest::startToQueryResult(info, bitrate);
}

void MusicKGQueryArtistRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << "downLoadFinished");

    MusicQueryArtistRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                bool artistFound = false;
                value = value["data"].toMap();

                const QVariantList &datas = value["info"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    TTK::MusicSongInformation info;
                    info.m_songName = TTK::String::charactersReplace(value["filename"].toString());
                    info.m_duration = TTKTime::formatDuration(value["duration"].toInt() * TTK_DN_S2MS);

                    if(info.m_songName.contains(TTK_DEFAULT_STR))
                    {
                        const QStringList &ll = info.m_songName.split(TTK_DEFAULT_STR);
                        info.m_singerName = TTK::String::charactersReplace(ll.front().trimmed());
                        info.m_songName = TTK::String::charactersReplace(ll.back().trimmed());
                    }

                    info.m_songId = value["hash"].toString();
                    info.m_albumId = value["album_id"].toString();

                    info.m_year.clear();
                    info.m_trackNumber = "0";

                    MusicResultDataItem albumInfo;
                    TTK_NETWORK_QUERY_CHECK();
                    MusicKGInterface::parseFromSongAlbumInfo(&albumInfo, info.m_albumId);
                    info.m_albumName = albumInfo.m_nickName;
                    TTK_NETWORK_QUERY_CHECK();
                    MusicKGInterface::parseFromSongLrcAndPicture(&info);
                    TTK_NETWORK_QUERY_CHECK();
                    MusicKGInterface::parseFromSongProperty(&info, value);
                    TTK_NETWORK_QUERY_CHECK();


                    if(!artistFound)
                    {
                        artistFound = true;
                        MusicResultDataItem result;
                        TTK_NETWORK_QUERY_CHECK();
                        queryArtistIntro(&result);
                        TTK_NETWORK_QUERY_CHECK();

                        result.m_id = m_queryValue;
                        result.m_name = info.m_singerName;
                        result.m_coverUrl = info.m_coverUrl;
                        Q_EMIT createArtistItem(result);
                    }

                    MusicResultInfoItem item;
                    item.m_songName = info.m_songName;
                    item.m_singerName = info.m_singerName;
                    item.m_albumName = info.m_albumName;
                    item.m_duration = info.m_duration;
                    item.m_type = mapQueryServerString();
                    Q_EMIT createSearchedItem(item);
                    m_songInfos << info;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void MusicKGQueryArtistRequest::queryArtistIntro(MusicResultDataItem *item) const
{
    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_ARTIST_INFO_URL, false).arg(m_queryValue));
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
        if(value["errcode"].toInt() == 0)
        {
            value = value["data"].toMap();

            item->m_nickName = value["alias"].toString();
            item->m_description = value["intro"].toString();

            if(item->m_nickName.isEmpty())
            {
                item->m_nickName = TTK_DEFAULT_STR;
            }
        }
    }
}
