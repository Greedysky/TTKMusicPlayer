#include "musicwyqueryalbumrequest.h"

static constexpr const char *WY_ALBUM_URL = "QzJrMDNKTGlpVHpvY2taYm5BSWZGVTZWM09xYW5STDdXRFE5eTEzczhLZHB1RWljWlIxOVhRUXVDcmpuVjJsQnhqcmZRZE9UU0JZPQ==";
static constexpr const char *WY_ARTIST_ALBUM_URL = "dEJpSTdiaUh3R1hnRGVDUlpCNzJKRnBqNWZHY2xLVmhUcWQ0WW1PUVY0NjlONmRvMmtGaWZiMDBscjAvd0dxZg==";
static constexpr const char *WY_ARTIST_ALBUM_DATA_URL = "Smx6dWVoWEJJOGEvdlZvRTNtcUFEVEJzWjI5NzB4bUM1anV4Z2lXa1N4RHRtTW5ZRllEeGpHSzN0VmxTTzJ4ZU93dW9Edz09";
static constexpr const char *WY_NEW_ALBUM_URL = "VC9sOTJXRzdLc0dYWWVtbmdsSUJnNFlYeDFqZ2gyd3Y5c1ZNdjBEeldpR25qSExSWWJqdVRkWmdoSmM9";
static constexpr const char *WY_NEW_ALBUM_DATA_URL = "N1VpQldOeUpVZFp3REZNczdrOHZXK0JpbjJkYjEveENIQmdaYlNhOWkyWUlrRWVYSnhCZTdBRS9SenFVVG11d0JHVXZ0NW1kTEdaeG4xeFc=";

MusicWYQueryAlbumRequest::MusicWYQueryAlbumRequest(QObject *parent)
    : MusicQueryAlbumRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_30;
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicWYQueryAlbumRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_ALBUM_URL, false).arg(m_queryValue),
                      QString("{}"));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYQueryAlbumRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downloadFinished();
    TTK_NETWORK_QUERY_CHECK();
    ReqWYInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    fetchUrlPathSize(&info->m_songProps, info->m_duration);
    MusicQueryAlbumRequest::startToQueryResult(info, bitrate);
}

void MusicWYQueryAlbumRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicPageQueryRequest::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value["code"].toInt() == 200 && value.contains("album"))
            {
                MusicResultDataItem item;
                const QVariantMap &albumValue = value["album"].toMap();
                item.m_coverUrl = albumValue["picUrl"].toString();
                item.m_count = albumValue["name"].toString();
                item.m_description = albumValue["company"].toString();
                item.m_time = TTKDateTime::format(albumValue["publishTime"].toULongLong(), TTK_DATE_FORMAT);
                item.m_category = albumValue["language"].toString();

                const QString &time = TTKDateTime::format(albumValue["publishTime"].toULongLong(), TTK_YEAR_FORMAT);
                const QVariantList &datas = value["songs"].toList();
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
                    info.m_year = time;
                    info.m_trackNumber = value["no"].toString();

                    TTK_NETWORK_QUERY_CHECK();
                    ReqWYInterface::parseFromSongProperty(&info, value);
                    TTK_NETWORK_QUERY_CHECK();

                    if(!m_albumFound)
                    {
                        m_albumFound = true;
                        item.m_id = info.m_albumId;
                        item.m_name = info.m_artistName;
                        Q_EMIT createAlbumItem(item);
                    }

                    Q_EMIT createResultItem({info, serverToString()});
                    m_items << info;
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}



MusicWYQueryArtistAlbumRequest::MusicWYQueryArtistAlbumRequest(QObject *parent)
    : MusicQueryAlbumRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_12;
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicWYQueryArtistAlbumRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_ARTIST_ALBUM_URL, false).arg(m_queryValue),
                      TTK::Algorithm::mdII(WY_ARTIST_ALBUM_DATA_URL, false).arg(m_pageSize * offset).arg(m_pageSize));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYQueryArtistAlbumRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicPageQueryRequest::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value["code"].toInt() == 200 && value.contains("hotAlbums"))
            {
                const QVariantMap &artistValue = value["artist"].toMap();
                m_totalSize = artistValue["albumSize"].toInt();

                const QVariantList &datas = value["hotAlbums"].toList();
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
                    item.m_name = value["name"].toString();
                    item.m_coverUrl = ReqWYInterface::makeCoverPixmapUrl(value["picUrl"].toString());
                    item.m_time = TTKDateTime::format(value["publishTime"].toULongLong(), TTK_DATE2_FORMAT);
                    Q_EMIT createAlbumItem(item);
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}



MusicWYQueryNewAlbumRequest::MusicWYQueryNewAlbumRequest(QObject *parent)
    : MusicQueryAlbumRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_30;
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicWYQueryNewAlbumRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_pageIndex = offset;

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                       TTK::Algorithm::mdII(WY_NEW_ALBUM_URL, false),
                       TTK::Algorithm::mdII(WY_NEW_ALBUM_DATA_URL, false).arg(m_queryValue).arg(offset).arg(m_pageSize));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYQueryNewAlbumRequest::downloadFinished()
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
