#include "musicmainrecommendrequest.h"
#include "musicwyqueryinterface.h"


static constexpr const char *NEW_ALBUM_URL = "VC9sOTJXRzdLc0dYWWVtbmdsSUJnNFlYeDFqZ2gyd3Y5c1ZNdjBEeldpR25qSExSWWJqdVRkWmdoSmM9";
static constexpr const char *NEW_ALBUM_DATA_URL = "N1VpQldOeUpVZFp3REZNczdrOHZXK0JpbjJkYjEveENIQmdaYlNhOWkyWUlrRWVYSnhCZTdBRS9SenFVVG11d0JHVXZ0NW1kTEdaeG4xeFc=";

static constexpr const char *ARTIST_URL = "c1l3a204QlJmWnJ0WVhxOTZlNU9RdWJVeGZRWDIvaU83RWxWTTIrRW84aExRajVWWEZXMnJmZlowRFpmWFdEV2NoM29IQT09";
static constexpr const char *ARTIST_DATA_URL = "TG9pYmlKeWRiaG45TFlHalhUNWtzYUxDUEZZL0VVdGJEOFlTTkFuaWdmb3hDempSRUMwNkJmSkdpeFU9";


static constexpr const char *PLAYLIST_URL = "Z3N3cXVjb1ZoV2pxRVY1d1RoSStuMXp3MGsreUVPRVNtQjVNMUR2WXgwWHJrTHBlODYxcFJpUnozbGlCNTBzYkF4eFRNMDd0dHk2eEF3WkVnc3hVdlE9PQ==";
static constexpr const char *PLAYLIST_DATA_URL = "dEZwUWp1MTJ6a2xaNytOOWZ5U1h1dFB5V0pUbkl6TXR0d3BmazkvYTM3ND0=";

static constexpr const char *PLAYLIST_HQ_URL = "bUZ0V2JxeUFxWFF6U3MxVUcrZklyUGhPc245MEF2TVlHVnNGdnZNVjZTWVJ6RmN1ZGZZNGFScWJsWCt3b1h6YnhYSFA4ZVQ2SC85N1Y4eFp3ZVhjNkZuUlNHTT0=";
static constexpr const char *PLAYLIST_HQ_DATA_URL = "MUlkMGR1NU00VzZJbjh1algwclpzY1NNTngzREN3TEoxUExOWVVmbDdPUTZlKzJOeFkwUjdGeUlRaDNGd2JoSzhBcEdtVk5paVc1VnFOdEE=";


MusicNewAlbumsRecommendRequest::MusicNewAlbumsRecommendRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_30;
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicNewAlbumsRecommendRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_pageIndex = offset;

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                       TTK::Algorithm::mdII(NEW_ALBUM_URL, false),
                       TTK::Algorithm::mdII(NEW_ALBUM_DATA_URL, false).arg(m_queryValue).arg(offset).arg(m_pageSize));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicNewAlbumsRecommendRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    m_queryValue = value;
    startToPage(0);
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
                m_totalSize = value["total"].toInt();

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
    m_pageSize = TTK_PAGE_SIZE_30;
    m_queryServer = QUERY_WY_INTERFACE;
    m_totalSize = 100;
}

void MusicArtistsRecommendRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_pageIndex = offset;

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                       TTK::Algorithm::mdII(ARTIST_URL, false),
                       TTK::Algorithm::mdII(ARTIST_DATA_URL, false).arg(m_pageSize * offset).arg(m_pageSize));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicArtistsRecommendRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    m_queryValue = value;
    startToPage(0);
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
    m_pageSize = TTK_PAGE_SIZE_30;
    m_queryServer = QUERY_WY_INTERFACE;
    m_totalSize = m_pageSize;
}

void MusicPlaylistRecommendRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_pageIndex = offset;

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                       TTK::Algorithm::mdII(PLAYLIST_URL, false),
                       TTK::Algorithm::mdII(PLAYLIST_DATA_URL, false).arg(m_pageSize));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicPlaylistRecommendRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    m_queryValue = value;
    startToPage(0);
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
    m_pageSize = TTK_PAGE_SIZE_30;
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicPlaylistHQRecommendRequest::startToPage(int offset)
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
                       TTK::Algorithm::mdII(PLAYLIST_HQ_URL, false),
                       TTK::Algorithm::mdII(PLAYLIST_HQ_DATA_URL, false).arg(m_queryValue).arg(time).arg(m_pageSize));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicPlaylistHQRecommendRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    m_more = true;
    m_updateTime.clear();
    m_updateTime[0] = 0;
    m_pageIndex = 0;
    m_queryValue = value;
    startToPage(0);
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
