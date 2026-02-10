#include "musicwyqueryplaylistrequest.h"

static constexpr const char *WY_PLAYLIST_URL = "aFZGMG5CbXZXNmNGaS9jZEV6S0c2RE53Tm5Ud3I3WGJSS2ErQVQ5RjdYRlhUMENuS25yRnYwVHUzMnIrQUJQaFRVTlMrSTJUM1pNPQ==";
static constexpr const char *WY_PLAYLIST_DATA_URL = "NXA2YlMvSVEyRWhvckRqclAzdDdreHY5L0JMb0tYS2tJRGhnRDlidVRUcHBFR0I5b3dXb1pPU0dDWGdpMjc3VDBpaUpBT05oNnlOc2czZmgxVGVpR2dTR01pbWhsVUdFRWhWYnpscUkzeTA9";
static constexpr const char *WY_PLAYLIST_INFO_V1_URL = "VUdTa0xMeG1Nblc4cThoS2E1QzhJK0lrTHJBRmk4TUJZMC9NQW1GanZydDJmR0VJVVRZNmJQSEhOZU96aE55bTJxdW55RldJOHl6aElNdTlHbkZkSmc9PQ==";
static constexpr const char *WY_PLAYLIST_INFO_V1_DATA_URL = "VkpFemFKQWtOY2ZXeUNpZ210VjVGYndOZ2VibUZlZHV6amZJaE5VL1czaWFzRkNqV2lYKy9CR29McXl6LzUrUldWcmtKS0Fjb0s5dFBFeXpCaHRCc1dhYlp1bGd3U1ZY";
static constexpr const char *WY_PLAYLIST_INFO_V2_URL = "VENmNTdHK2VacjVvTFp3WmFKT2wrNmNnWHM1N3FDUHNGUHZ4MWdvU2x2bzdMM0xCSXFaaFRWWnNYTkpLeGdnVw==";
static constexpr const char *WY_PLAYLIST_INFO_V2_DATA_URL = "cTlvTkRSVU9ncjYvTDBJaDZldWtNTXh0d1dLcmd6N0k=";
static constexpr const char *WY_PLAYLIST_RECOMMED_URL = "Z3N3cXVjb1ZoV2pxRVY1d1RoSStuMXp3MGsreUVPRVNtQjVNMUR2WXgwWHJrTHBlODYxcFJpUnozbGlCNTBzYkF4eFRNMDd0dHk2eEF3WkVnc3hVdlE9PQ==";
static constexpr const char *WY_PLAYLIST_RECOMMED_DATA_URL = "dEZwUWp1MTJ6a2xaNytOOWZ5U1h1dFB5V0pUbkl6TXR0d3BmazkvYTM3ND0=";
static constexpr const char *WY_PLAYLIST_HQ_URL = "bUZ0V2JxeUFxWFF6U3MxVUcrZklyUGhPc245MEF2TVlHVnNGdnZNVjZTWVJ6RmN1ZGZZNGFScWJsWCt3b1h6YnhYSFA4ZVQ2SC85N1Y4eFp3ZVhjNkZuUlNHTT0=";
static constexpr const char *WY_PLAYLIST_HQ_DATA_URL = "MUlkMGR1NU00VzZJbjh1algwclpzY1NNTngzREN3TEoxUExOWVVmbDdPUTZlKzJOeFkwUjdGeUlRaDNGd2JoSzhBcEdtVk5paVc1VnFOdEE=";

MusicWYQueryPlaylistRequest::MusicWYQueryPlaylistRequest(QObject *parent)
    : MusicQueryPlaylistRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_30;
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
                    info.m_year = TTKDateTime::format(value["publishTime"].toULongLong(), TTK_YEAR_FORMAT);
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



MusicWYQueryPlaylistRecommendRequest::MusicWYQueryPlaylistRecommendRequest(QObject *parent)
    : MusicQueryPlaylistRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_30;
    m_queryServer = QUERY_WY_INTERFACE;
    m_totalSize = m_pageSize;
}

void MusicWYQueryPlaylistRecommendRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_pageIndex = offset;

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                       TTK::Algorithm::mdII(WY_PLAYLIST_RECOMMED_URL, false),
                       TTK::Algorithm::mdII(WY_PLAYLIST_RECOMMED_DATA_URL, false).arg(m_pageSize));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYQueryPlaylistRecommendRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    m_queryValue = value;
    startToPage(0);
}

void MusicWYQueryPlaylistRecommendRequest::startToQueryInfo(MusicResultDataItem &item)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << item.m_id);
}

void MusicWYQueryPlaylistRecommendRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicAbstractQueryRequest::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value["code"].toInt() == 200 && value.contains("result"))
            {
                const QVariantList &datas = value["result"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultDataItem item;
                    item.m_coverUrl = value["picUrl"].toString();
                    item.m_id = value["id"].toString();
                    item.m_name = value["name"].toString();
                    item.m_nickName = "热门歌单推荐";
                    Q_EMIT createPlaylistItem(item);
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}



MusicWYQueryPlaylistHighRequest::MusicWYQueryPlaylistHighRequest(QObject *parent)
    : MusicQueryPlaylistRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_30;
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicWYQueryPlaylistHighRequest::startToPage(int offset)
{
    if(offset == 0)
    {
        if(--m_pageIndex < 0)
        {
            m_pageIndex = 0;
        }
    }
    else if(m_more && m_updateTime.contains(m_pageIndex + 1))
    {
        ++m_pageIndex;
    }

    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << m_pageIndex);

    deleteAll();
    const qint64 time = m_updateTime[m_pageIndex];

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                       TTK::Algorithm::mdII(WY_PLAYLIST_HQ_URL, false),
                       TTK::Algorithm::mdII(WY_PLAYLIST_HQ_DATA_URL, false).arg(m_queryValue).arg(time).arg(m_pageSize));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYQueryPlaylistHighRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    switch(value.toInt())
    {
        case 0: m_queryValue = "全部"; break;
        case 1: m_queryValue = "华语"; break;
        case 2: m_queryValue = "欧美"; break;
        case 3: m_queryValue = "韩语"; break;
        case 4: m_queryValue = "日语"; break;
        default: break;
    }

    m_more = true;
    m_updateTime.clear();
    m_updateTime[0] = 0;
    m_pageIndex = 0;
    startToPage(0);
}

void MusicWYQueryPlaylistHighRequest::startToQueryInfo(MusicResultDataItem &item)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << item.m_id);
}

void MusicWYQueryPlaylistHighRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicAbstractQueryRequest::downloadFinished();
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
                if(m_totalSize > m_pageSize * 2)
                {
                    m_totalSize = m_pageSize * 2;
                }

                m_more = value["more"].toBool();
                m_updateTime[m_pageIndex + 1] = value["lasttime"].toLongLong();

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

                    value = value["creator"].toMap();
                    item.m_nickName = value["nickname"].toString();
                    Q_EMIT createPlaylistItem(item);
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}
