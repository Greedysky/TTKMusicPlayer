#include "musickgqueryalbumrequest.h"

static constexpr const char *KG_ALBUM_URL = "SnJGeC9vRFdybkgyQnNLMUpvaFN4c0hLMDhoVW9UVmRFNExWWW9ZMk1EaEw2cGFwend6eWJEMFgzdHkxYnRFNXVYUUhaVXp4bFE4UklTWVpNZ05CY29KYUVNYzU3S3M3T1NERFpjR0JMWURYNUpOMWJMTGZmRmh0NkdjVlNhS254YUp6eUE9PQ==";
static constexpr const char *KG_ARTIST_ALBUM_URL = "OWJUSnVoMWRJY0FONGRTVnRoSlJ1ZkNUQjNPZnBjKzgrQzg1K0RVczJsaWprZUpXMnk1U0Z4Vm5BcXNKVlR5K0U0aWptd0hUME41eXdPa0xPMGg4Z01HTTBGVWFmTFM4WUNQaFduU3FYUXM9";
static constexpr const char *KG_NEW_ALBUM_URL = "RTJzNjFFRkdzNjdKTy9XcHB2enJ5amFFMUpObXpyMG05R2EyeXRFczREUWlRR3RYL3p5T3BIejArTHJFQ0QyVEdhYkIwa09KalBBdHZaclczcWFqbElmM1Fjbz0=";

MusicKGQueryAlbumRequest::MusicKGQueryAlbumRequest(QObject *parent)
    : MusicQueryAlbumRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_30;
    m_queryServer = QUERY_KG_INTERFACE;
}

void MusicKGQueryAlbumRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_ALBUM_URL, false).arg(m_queryValue).arg(offset + 1).arg(m_pageSize));
    ReqKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGQueryAlbumRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downloadFinished();
    TTK_NETWORK_QUERY_CHECK();
    ReqKGInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    fetchUrlPathSize(&info->m_songProps, info->m_duration);
    MusicQueryAlbumRequest::startToQueryResult(info, bitrate);
}

void MusicKGQueryAlbumRequest::downloadFinished()
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
            if(value.contains("data"))
            {
                QString albumName;
                value = value["data"].toMap();
                m_totalSize = value["total"].toInt();

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
                    info.m_songId = value["hash"].toString();

                    info.m_albumId = value["album_id"].toString();

                    info.m_duration = TTKTime::formatDuration(value["duration"].toInt() * TTK_DN_S2MS);
                    info.m_year.clear();
                    info.m_trackNumber = "0";

                    TTK_NETWORK_QUERY_CHECK();
                    ReqKGInterface::parseFromSongAlbumLrc(&info);
                    TTK_NETWORK_QUERY_CHECK();
                    ReqKGInterface::parseFromSongProperty(&info, value);
                    TTK_NETWORK_QUERY_CHECK();

                    if(!m_albumFound)
                    {
                        m_albumFound = true;
                        MusicResultDataItem item;
                        TTK_NETWORK_QUERY_CHECK();
                        ReqKGInterface::parseFromSongAlbumInfo(&item, info.m_songId, value["album_audio_id"].toString());
                        TTK_NETWORK_QUERY_CHECK();

                        albumName = item.m_name;
                        item.m_count = item.m_name;
                        item.m_id = info.m_albumId;
                        item.m_name = info.m_artistName;
                        item.m_coverUrl = info.m_coverUrl;
                        Q_EMIT createAlbumItem(item);
                    }

                    info.m_albumName = albumName;
                    Q_EMIT createResultItem({info, serverToString()});
                    m_items << info;
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}



MusicKGQueryArtistAlbumRequest::MusicKGQueryArtistAlbumRequest(QObject *parent)
    : MusicQueryAlbumRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_12;
    m_queryServer = QUERY_KG_INTERFACE;
}

void MusicKGQueryArtistAlbumRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_ARTIST_ALBUM_URL, false).arg(m_queryValue).arg(offset + 1).arg(m_pageSize));
    ReqKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGQueryArtistAlbumRequest::downloadFinished()
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
            if(value["errcode"].toInt() == 0 && value.contains("data"))
            {
                value = value["data"].toMap();
                m_totalSize = value["total"].toInt();

                const QVariantList &datas = value["info"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultDataItem item;
                    item.m_id = value["albumid"].toString();
                    item.m_name = value["albumname"].toString();
                    item.m_coverUrl = value["imgurl"].toString().replace("{size}", "400");
                    item.m_time = TTK::String::split(value["publishtime"].toString().replace(TTK_DEFAULT_STR, TTK_DOT), TTK_SPACE).first();
                    Q_EMIT createAlbumItem(item);
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}



MusicKGQueryNewAlbumRequest::MusicKGQueryNewAlbumRequest(QObject *parent)
    : MusicQueryAlbumRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_30;
    m_queryServer = QUERY_KG_INTERFACE;
}

void MusicKGQueryNewAlbumRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_NEW_ALBUM_URL, false).arg(offset + 1).arg(m_pageSize));
    ReqKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGQueryNewAlbumRequest::downloadFinished()
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
            if(value["errcode"].toInt() == 0 && value.contains("data"))
            {
                value = value["data"].toMap();
                m_totalSize = value["total"].toInt();

                const QVariantList &datas = value["info"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultDataItem item;
                    item.m_id = value["albumid"].toString();
                    item.m_name = value["albumname"].toString();
                    item.m_coverUrl = value["imgurl"].toString().replace("{size}", "400");
                    item.m_nickName = value["singername"].toString();
                    Q_EMIT createAlbumItem(item);
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}
