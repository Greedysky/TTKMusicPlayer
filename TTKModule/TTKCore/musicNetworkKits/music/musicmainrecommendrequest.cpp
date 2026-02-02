#include "musicmainrecommendrequest.h"
#include "musicwyqueryinterface.h"

static constexpr const char *NEW_SONG_URL = "SnJhQ3VyZzNwU0ZnYlo2ME9BRVFjdmp0aGNTTldyL2c2RmtES3dFSkY4b0NFZGUwWDRteFpDS0F0SUozOU9nR0RVVE9lTUFwMEhNPQ==";
static constexpr const char *NEW_SONG_DATA_URL = "UnNXcXM0ODFxamFPVmFWeEM3Tm0xL1dZNlZTdDBib3QzejhwUXlsNEg4OGI2aDl4";
static constexpr const char *NEW_ALBUM_URL = "VC9sOTJXRzdLc0dYWWVtbmdsSUJnNFlYeDFqZ2gyd3Y5c1ZNdjBEeldpR25qSExSWWJqdVRkWmdoSmM9";
static constexpr const char *NEW_ALBUM_DATA_URL = "N1VpQldOeUpVZFp3REZNczdrOHZXK0JpbjJkYjEveENIQmdaYlNhOWkyWUlrRWVYSnhCZTdBRS9SenFVVG11d0JHVXZ0NW1kTEdaeG4xeFc=";
static constexpr const char *ARTIST_URL = "c1l3a204QlJmWnJ0WVhxOTZlNU9RdWJVeGZRWDIvaU83RWxWTTIrRW84aExRajVWWEZXMnJmZlowRFpmWFdEV2NoM29IQT09";
static constexpr const char *ARTIST_DATA_URL = "TG9pYmlKeWRiaG45TFlHalhUNWtzYUxDUEZZL0VVdGJEOFlTTkFuaWdmb3hDempSRUMwNkJmSkdpeFU9";
static constexpr const char *PLAYLIST_URL = "Z3N3cXVjb1ZoV2pxRVY1d1RoSStuMXp3MGsreUVPRVNtQjVNMUR2WXgwWHJrTHBlODYxcFJpUnozbGlCNTBzYkF4eFRNMDd0dHk2eEF3WkVnc3hVdlE9PQ==";
static constexpr const char *PLAYLIST_DATA_URL = "dEZwUWp1MTJ6a2xaNytOOWZ5U1h1dFB5V0pUbkl6TXR0d3BmazkvYTM3ND0=";
static constexpr const char *PLAYLIST_HQ_URL = "bUZ0V2JxeUFxWFF6U3MxVUcrZklyUGhPc245MEF2TVlHVnNGdnZNVjZTWVJ6RmN1ZGZZNGFScWJsWCt3b1h6YnhYSFA4ZVQ2SC85N1Y4eFp3ZVhjNkZuUlNHTT0=";
static constexpr const char *PLAYLIST_HQ_DATA_URL = "MUlkMGR1NU00VzZJbjh1algwclpzY1NNTngzREN3TEoxUExOWVVmbDdPUTZlKzJOeFkwUjdGeUlRaDNGd2JoSzhBcEdtVk5paVc1VnFOdEE=";

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
                       TTK::Algorithm::mdII(NEW_SONG_URL, false),
                       TTK::Algorithm::mdII(NEW_SONG_DATA_URL, false).arg(value));

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
    m_pageSize = 8;
    m_pageIndex = 0;
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicNewAlbumsRecommendRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    deleteAll();

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                       TTK::Algorithm::mdII(NEW_ALBUM_URL, false),
                       TTK::Algorithm::mdII(NEW_ALBUM_DATA_URL, false).arg(value).arg(m_pageIndex).arg(m_pageSize));

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


MusicArtistsRecommendRequest::MusicArtistsRecommendRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{
    m_pageSize = 8;
    m_pageIndex = 0;
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicArtistsRecommendRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    deleteAll();

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                       TTK::Algorithm::mdII(ARTIST_URL, false),
                       TTK::Algorithm::mdII(ARTIST_DATA_URL, false).arg(m_pageIndex).arg(m_pageSize));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicArtistsRecommendRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downloadFinished();
    TTK_NETWORK_QUERY_CHECK();
    ReqWYInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    fetchUrlPathSize(&info->m_songProps, info->m_duration);
    MusicAbstractQueryRequest::startToQueryResult(info, bitrate);
}

void MusicArtistsRecommendRequest::downloadFinished()
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
            if(value["code"].toInt() == 200 && value.contains("artists"))
            {
                const QVariantList &datas = value["artists"].toList();
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
                    Q_EMIT createArtistItem(item);
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
    m_pageSize = 8;
    m_pageIndex = 0;
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicPlaylistRecommendRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    deleteAll();

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                       TTK::Algorithm::mdII(PLAYLIST_URL, false),
                       TTK::Algorithm::mdII(PLAYLIST_DATA_URL, false).arg(m_pageSize));

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
                    item.m_nickName = "热门歌单推荐";
                    Q_EMIT createPlaylistItem(item);
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}


MusicPlaylistHQRecommendRequest::MusicPlaylistHQRecommendRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{
    m_pageSize = 8;
    m_pageIndex = 0;
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicPlaylistHQRecommendRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    deleteAll();

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                       TTK::Algorithm::mdII(PLAYLIST_HQ_URL, false),
                       TTK::Algorithm::mdII(PLAYLIST_HQ_DATA_URL, false).arg(value).arg(m_pageIndex).arg(m_pageSize));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicPlaylistHQRecommendRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downloadFinished();
    TTK_NETWORK_QUERY_CHECK();
    ReqWYInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    fetchUrlPathSize(&info->m_songProps, info->m_duration);
    MusicAbstractQueryRequest::startToQueryResult(info, bitrate);
}

void MusicPlaylistHQRecommendRequest::downloadFinished()
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
