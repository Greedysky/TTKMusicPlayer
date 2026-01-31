#include "musicmainrecommendrequest.h"
#include "musicwyqueryinterface.h"

MusicNewSongsRecommendRequest::MusicNewSongsRecommendRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{
    m_queryServer = QUERY_WY_INTERFACE;
    m_pageSize = 1;
    m_totalSize = 13;
}

void MusicNewSongsRecommendRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    deleteAll();

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                       TTK::Algorithm::mdII("SnJhQ3VyZzNwU0ZnYlo2ME9BRVFjdmp0aGNTTldyL2c2RmtES3dFSkY4b0NFZGUwWDRteFpDS0F0SUozOU9nR0RVVE9lTUFwMEhNPQ==", false),
                       TTK::Algorithm::mdII("ZTlMYXk1aGFqNHl0ZElzc0liZjZCbmJsSzVVL1ZNbEltODJxZXpIS1pvST0=", false));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicNewSongsRecommendRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downloadFinished();
    TTK_NETWORK_QUERY_CHECK();
    ReqWYInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    fetchUrlPathSize(&info->m_songProps, info->m_duration);
    MusicAbstractQueryRequest::startToQueryResult(info, bitrate);
}

void MusicNewSongsRecommendRequest::downloadFinished()
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
            if(value["code"].toInt() == 200 && value.contains("data"))
            {
                value = value["data"].toMap();

                const QVariantList &datas = value["newList"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    if(!pageValid())
                    {
                        break;
                    }

                    ++m_pageIndex;

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
    deleteAll();
}


MusicNewAlbumsRecommendRequest::MusicNewAlbumsRecommendRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicNewAlbumsRecommendRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    deleteAll();

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                       TTK::Algorithm::mdII("VC9sOTJXRzdLc0dYWWVtbmdsSUJnNFlYeDFqZ2gyd3Y5c1ZNdjBEeldpR25qSExSWWJqdVRkWmdoSmM9", false),
                       TTK::Algorithm::mdII("SUt5RXJmaFJJbWVNeXJRY3FidVlpdmJDK1U3ZTA0a0pvaXNLRWdhd05sTW9vVklrcldNclJvbGhKTEgwOVBGN0puU3NURXNORFFVPQ==", false));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicNewAlbumsRecommendRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downloadFinished();
    TTK_NETWORK_QUERY_CHECK();
    ReqWYInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    fetchUrlPathSize(&info->m_songProps, info->m_duration);
    MusicAbstractQueryRequest::startToQueryResult(info, bitrate);
}

void MusicNewAlbumsRecommendRequest::downloadFinished()
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
            if(value["code"].toInt() == 200 && value.contains("albums"))
            {
                const QVariantList &datas = value["albums"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultDataItem item;
                    item.m_id = value["id"].toString();
                    item.m_name = TTK::String::charactersReplace(value["name"].toString());
                    item.m_coverUrl = value["picUrl"].toString();
                    item.m_time = TTKDateTime::format(value["publishTime"].toULongLong(), TTK_YEAR_FORMAT);
                    item.m_nickName.clear();

                    const QVariantList &artistsArray = value["artists"].toList();
                    for(const QVariant &artistValue : qAsConst(artistsArray))
                    {
                        if(artistValue.isNull())
                        {
                            continue;
                        }

                        const QVariantMap &artistObject = artistValue.toMap();
                        item.m_nickName = ReqWYInterface::makeSongArtist(item.m_nickName, artistObject["name"].toString());
                    }

                    Q_EMIT createAlbumItem(item);
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}


MusicPlaylistRecommendRequest::MusicPlaylistRecommendRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicPlaylistRecommendRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    deleteAll();

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                       TTK::Algorithm::mdII("Z3N3cXVjb1ZoV2pxRVY1d1RoSStuMXp3MGsreUVPRVNtQjVNMUR2WXgwWHJrTHBlODYxcFJpUnozbGlCNTBzYkF4eFRNMDd0dHk2eEF3WkVnc3hVdlE9PQ==", false),
                       TTK::Algorithm::mdII("ZzVpb3g5TFJVc0NoVjNXem5ybHduQ0pTR1JaMm9ZeWQ5dUJQNlA4Z0VLUT0=", false));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicPlaylistRecommendRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downloadFinished();
    TTK_NETWORK_QUERY_CHECK();
    ReqWYInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    fetchUrlPathSize(&info->m_songProps, info->m_duration);
    MusicAbstractQueryRequest::startToQueryResult(info, bitrate);
}

void MusicPlaylistRecommendRequest::downloadFinished()
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
                    item.m_count = value["playCount"].toString();
                    item.m_time = TTKDateTime::format(value["trackNumberUpdateTime"].toULongLong(), TTK_DATE_FORMAT);
                    item.m_category.clear();
                    Q_EMIT createPlaylistItem(item);
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}


MusicPlaylistHighqualityRecommendRequest::MusicPlaylistHighqualityRecommendRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicPlaylistHighqualityRecommendRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    deleteAll();

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                       "https://interface.music.163.com/weapi/playlist/highquality/list",
                       "{\"cat\": \"全部\",\"lasttime\": 0,\"limit\": 8,\"total\": true}");

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicPlaylistHighqualityRecommendRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downloadFinished();
    TTK_NETWORK_QUERY_CHECK();
    ReqWYInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    fetchUrlPathSize(&info->m_songProps, info->m_duration);
    MusicAbstractQueryRequest::startToQueryResult(info, bitrate);
}

void MusicPlaylistHighqualityRecommendRequest::downloadFinished()
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

    Q_EMIT downloadDataChanged({});
    deleteAll();
}
