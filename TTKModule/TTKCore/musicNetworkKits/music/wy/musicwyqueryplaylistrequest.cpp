#include "musicwyqueryplaylistrequest.h"

MusicWYQueryPlaylistRequest::MusicWYQueryPlaylistRequest(QObject *parent)
    : MusicQueryPlaylistRequest(parent)
{
    m_pageSize = 30;
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicWYQueryPlaylistRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(className() << "startToPage" << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    const QByteArray &parameter = MusicWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_PLAYLIST_URL, false),
                      TTK::Algorithm::mdII(WY_PLAYLIST_DATA_URL, false).arg(m_queryValue).arg(m_pageSize * offset).arg(m_pageSize));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYQueryPlaylistRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(className() << "startToSearch" << value);

    deleteAll();

    QNetworkRequest request;
    const QByteArray &parameter = MusicWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_PLAYLIST_INFO_URL, false),
                      TTK::Algorithm::mdII(WY_PLAYLIST_INFO_DATA_URL, false).arg(value));

    QNetworkReply *reply = m_manager.post(request, parameter);
    connect(reply, SIGNAL(finished()), SLOT(downloadDetailsFinished()));
    QtNetworkErrorConnect(reply, this, replyError, TTK_SLOT);
}

void MusicWYQueryPlaylistRequest::startToSingleSearch(const QString &value)
{
    TTK_INFO_STREAM(className() << "startToSingleSearch" << value);

    m_queryValue = value.isEmpty() ? "all" : value;
    startToPage(0);
}

void MusicWYQueryPlaylistRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(className() << "startToQueryResult" << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downLoadFinished();
    TTK_NETWORK_QUERY_CHECK();
    MusicWYInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();
    MusicQueryPlaylistRequest::startToQueryResult(info, bitrate);
}

void MusicWYQueryPlaylistRequest::startToQueryInfo(MusicResultDataItem &item)
{
    TTK_INFO_STREAM(className() << "startToQueryInfo" << item.m_id);

    MusicPageQueryRequest::downLoadFinished();

    QNetworkRequest request;
    const QByteArray &parameter = MusicWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_PLAYLIST_INFO_URL, false),
                      TTK::Algorithm::mdII(WY_PLAYLIST_INFO_DATA_URL, false).arg(item.m_id));

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
        if(value["code"].toInt() == 200 && value.contains("playlist"))
        {
            value = value["playlist"].toMap();
            item.m_coverUrl = value["coverImgUrl"].toString();
            item.m_name = value["name"].toString();
            item.m_count = value["playCount"].toString();
            item.m_description = value["description"].toString();
            item.m_updateTime = TTKDateTime::format(value["updateTime"].toULongLong(), TTK_YEAR_FORMAT);
            item.m_category.clear();

            const QVariantList &tags = value["tags"].toList();
            for(const QVariant &var : qAsConst(tags))
            {
                if(var.isNull())
                {
                    continue;
                }

                item.m_category.append(var.toString() + "|");
            }

            value = value["creator"].toMap();
            item.m_nickName = value["nickname"].toString();
        }
    }
}

void MusicWYQueryPlaylistRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << "downLoadFinished");

    MusicQueryPlaylistRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 200 && value.contains("playlists"))
            {
                m_totalSize = value["total"].toInt();

                const QVariantList &datas = value["playlists"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultDataItem result;
                    result.m_coverUrl = value["coverImgUrl"].toString();
                    result.m_id = value["id"].toString();
                    result.m_name = value["name"].toString();
                    result.m_count = value["playCount"].toString();
                    result.m_description = value["description"].toString();
                    result.m_updateTime = TTKDateTime::format(value["updateTime"].toULongLong(), TTK_YEAR_FORMAT);
                    result.m_category.clear();

                    const QVariantList &tags = value["tags"].toList();
                    for(const QVariant &var : qAsConst(tags))
                    {
                        if(var.isNull())
                        {
                            continue;
                        }
                        result.m_category.append(var.toString() + "|");
                    }

                    value = value["creator"].toMap();
                    result.m_nickName = value["nickname"].toString();
                    Q_EMIT createPlaylistItem(result);
                }
            }
        }
    }

//    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void MusicWYQueryPlaylistRequest::downloadDetailsFinished()
{
    TTK_INFO_STREAM(className() << "downloadDetailsFinished");

    MusicQueryPlaylistRequest::downLoadFinished();
    QNetworkReply *reply = TTKObjectCast(QNetworkReply*, sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 200 && value.contains("playlist"))
            {
                value = value["playlist"].toMap();

                const QVariantList &datas = value["tracks"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    TTK::MusicSongInformation info;
                    info.m_songName = TTK::String::charactersReplace(value["name"].toString());
                    info.m_duration = TTKTime::formatDuration(value["dt"].toInt());
                    info.m_songId = value["id"].toString();
                    info.m_lrcUrl = TTK::Algorithm::mdII(WY_SONG_LRC_OLD_URL, false).arg(info.m_songId);

                    const QVariantMap &albumObject = value["al"].toMap();
                    info.m_coverUrl = albumObject["picUrl"].toString();
                    info.m_albumId = albumObject["id"].toString();
                    info.m_albumName = TTK::String::charactersReplace(albumObject["name"].toString());

                    const QVariantList &artistsArray = value["ar"].toList();
                    for(const QVariant &artistValue : qAsConst(artistsArray))
                    {
                        if(artistValue.isNull())
                        {
                            continue;
                        }

                        const QVariantMap &artistObject = artistValue.toMap();
                        info.m_artistId = artistObject["id"].toString();
                        info.m_singerName = TTK::String::charactersReplace(artistObject["name"].toString());
                        break; //just find first singer
                    }

                    info.m_year.clear();
                    info.m_trackNumber = value["no"].toString();

                    TTK_NETWORK_QUERY_CHECK();
                    MusicWYInterface::parseFromSongProperty(&info, value);
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultInfoItem item;
                    item.m_songName = info.m_songName;
                    item.m_singerName = info.m_singerName;
                    item.m_albumName = info.m_albumName;
                    item.m_duration = info.m_duration;
                    item.m_type = serverToString();
                    Q_EMIT createSearchedItem(item);
                    m_songInfos << info;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
}
