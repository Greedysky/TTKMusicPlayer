#include "musicwyqueryplaylistrequest.h"

MusicWYQueryPlaylistRequest::MusicWYQueryPlaylistRequest(QObject *parent)
    : MusicQueryPlaylistRequest(parent)
{
    m_pageSize = SONG_PAGE_SIZE;
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicWYQueryPlaylistRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_PLAYLIST_URL, false),
                      TTK::Algorithm::mdII(WY_PLAYLIST_DATA_URL, false).arg(m_queryValue).arg(m_pageSize * offset).arg(m_pageSize));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYQueryPlaylistRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    deleteAll();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(WY_PLAYLIST_INFO_V2_URL, false));
    ReqWYInterface::makeRequestRawHeader(&request);

    QNetworkReply *reply = m_manager.post(request, TTK::Algorithm::mdII(WY_PLAYLIST_INFO_V2_DATA_URL, false).arg(value).toUtf8());
    connect(reply, SIGNAL(finished()), SLOT(downloadDetailsFinished()));
    QtNetworkErrorConnect(reply, this, replyError, TTK_SLOT);
}

void MusicWYQueryPlaylistRequest::startToSearchByID(const QString &value)
{
    MusicQueryPlaylistRequest::startToSearchByID(value.isEmpty() ? "all" : value);
}

void MusicWYQueryPlaylistRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downloadFinished();
    TTK_NETWORK_QUERY_CHECK();
    ReqWYInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    fetchUrlPathSize(&info->m_songProps, info->m_duration);
    MusicQueryPlaylistRequest::startToQueryResult(info, bitrate);
}

void MusicWYQueryPlaylistRequest::startToQueryInfo(MusicResultDataItem &item)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << item.m_id);

    MusicPageQueryRequest::downloadFinished();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(WY_PLAYLIST_INFO_V2_URL, false));
    ReqWYInterface::makeRequestRawHeader(&request);

    const QByteArray &parameter = TTK::Algorithm::mdII(WY_PLAYLIST_INFO_V2_DATA_URL, false).arg(item.m_id).toUtf8();
    const QByteArray &bytes = TTK::syncNetworkQueryForPost(&request, parameter);
    if(bytes.isEmpty())
    {
        return;
    }

    QJsonParseError ok;
    const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
    if(QJsonParseError::NoError == ok.error)
    {
        QVariantMap value = json.toVariant().toMap();
        if(value["code"].toInt() == 200 && value.contains("playlist"))
        {
            value = value["playlist"].toMap();
            item.m_coverUrl = value["coverImgUrl"].toString();
            item.m_name = value["name"].toString();
            item.m_title = value["detailPageTitle"].toString();
            item.m_count = value["playCount"].toString();
            item.m_description = value["description"].toString();
            item.m_time = TTKDateTime::format(value["updateTime"].toULongLong(), TTK_DATE_FORMAT);
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

            if(!item.m_category.isEmpty())
            {
                item.m_category.insert(0, "|");
            }

            value = value["creator"].toMap();
            item.m_nickName = value["nickname"].toString();
        }
    }
}

void MusicWYQueryPlaylistRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicQueryPlaylistRequest::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
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

                    MusicResultDataItem item;
                    item.m_coverUrl = value["coverImgUrl"].toString();
                    item.m_id = value["id"].toString();
                    item.m_name = value["name"].toString();
                    item.m_count = value["playCount"].toString();
                    item.m_description = value["description"].toString();
                    item.m_time = TTKDateTime::format(value["updateTime"].toULongLong(), TTK_DATE_FORMAT);
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

                    if(!item.m_category.isEmpty())
                    {
                        item.m_category.insert(0, "|");
                    }

                    value = value["creator"].toMap();
                    item.m_nickName = value["nickname"].toString();
                    Q_EMIT createPlaylistItem(item);
                }
            }
        }
    }

//    Q_EMIT downloadDataChanged({});
    deleteAll();
}

void MusicWYQueryPlaylistRequest::downloadDetailsFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicQueryPlaylistRequest::downloadFinished();
    QNetworkReply *reply = TTKObjectCast(QNetworkReply*, sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
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
                    info.m_songId = value["id"].toString();
                    info.m_songName = TTK::String::charactersReplace(value["name"].toString());

                    const QVariantList &artistsArray = value["ar"].toList();
                    for(const QVariant &artistValue : qAsConst(artistsArray))
                    {
                        if(artistValue.isNull())
                        {
                            continue;
                        }

                        const QVariantMap &artistObject = artistValue.toMap();
                        if(info.m_artistId.isEmpty())
                        {
                            info.m_artistId = artistObject["id"].toString();
                        }

                        info.m_artistName = ReqWYInterface::makeSongArtist(info.m_artistName, artistObject["name"].toString());
                    }

                    const QVariantMap &albumObject = value["al"].toMap();
                    info.m_albumId = albumObject["id"].toString();
                    info.m_albumName = TTK::String::charactersReplace(albumObject["name"].toString());

                    info.m_coverUrl = albumObject["picUrl"].toString();
                    info.m_lrcUrl = TTK::Algorithm::mdII(WY_SONG_LRC_OLD_URL, false).arg(info.m_songId);
                    info.m_duration = TTKTime::formatDuration(value["dt"].toInt());
                    info.m_year.clear();
                    info.m_trackNumber = value["no"].toString();

                    TTK_NETWORK_QUERY_CHECK();
                    ReqWYInterface::parseFromSongProperty(&info, value);
                    TTK_NETWORK_QUERY_CHECK();

                    Q_EMIT createResultItem({info, serverToString()});
                    m_items << info;
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
}
