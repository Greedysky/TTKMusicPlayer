#include "musickwqueryalbumrequest.h"

static constexpr const char *KW_ALBUM_URL = "Nk5uY2lGelFqNHpteXVuekpQdFhpcWkvUnVRUWdZc1FOdUpXQkdYSWs3VmtDZU1PN29ZcUQ4dTNvczBhSmFMdkRLOGFJNnljSWJNc1QreU1TSU1SRldnN0RnRzlzWFBrUlNIbGlJUzFFelRsNzFoRUJEVkNLbHRzUnAwSzdtdEc2UFRmZEhSV0lvNVVSVXdjamhLT1dvTWxyZEE3aXlvQWFtQTg3Wmtrb05pZ0RIbXU5V3dxeWZLL3hQaFM5WmdKekRaRVhDL3VISVk9";
static constexpr const char *KW_ARTIST_ALBUM_URL = "YXNXaDl6TXNjWHhxeG9UeG9aaWR4aUhQRmpBR25VZ1NFQkZPbGNkdndGT09TbjRYV2ZzTHJJbUtmQjVvRVphQmltaFVUaG50M1dWTXZlWVJUOVNZVkJiVzF0amNVT3FJR09BRUUvVTU2Y1E5QTRiQ1M2SU5ES2MvdWpMeDFiT0dxWDErMkZpVyt5ZnNOeTc1K2dyRG5QVS9NVTRyN0hpNUUyVWVjZz09";
static constexpr const char *KW_NEW_ALBUM_URL = "SU5LT3ZXSExhQ2FEWWpFRmJxa1k1cjB6WktRc2IrNENiclo3KzVqUlBaNVRORzVoNHhlU2VheVA4VFhlaGxWaVg3SFJLQ0FmNWQ0dFdIQVVUTUxIUmU4RkJyTDRxSC9DL3kvZ0crZmZYWTlOR21WRlU0WFZaa1BFN3NGcVlIcXlXamUyWmg0UjlnT2hLVVU2bzdTb0QxaGQ3QmhkNWJXMStTTlFJVmFQK1c5TDZnclU=";

MusicKWQueryAlbumRequest::MusicKWQueryAlbumRequest(QObject *parent)
    : MusicQueryAlbumRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_30;
    m_queryServer = QUERY_KW_INTERFACE;
}

void MusicKWQueryAlbumRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_ALBUM_URL, false).arg(m_queryValue));
    ReqKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWQueryAlbumRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downloadFinished();
    TTK_NETWORK_QUERY_CHECK();
    ReqKWInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    fetchUrlPathSize(&info->m_songProps, info->m_duration);
    MusicQueryAlbumRequest::startToQueryResult(info, bitrate);
}

void MusicKWQueryAlbumRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicPageQueryRequest::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll().replace("'", "\""), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value.contains("musiclist"))
            {
                MusicResultDataItem item;
                item.m_id = value["albumid"].toString();
                item.m_count = value["name"].toString();
                item.m_description = value["company"].toString();
                item.m_time = value["pub"].toString();
                item.m_category = value["lang"].toString();
                item.m_coverUrl = ReqKWInterface::makeCoverPixmapUrl(value["pic"].toString(), {});

                m_totalSize = value["songnum"].toInt();

                const QVariantList &datas = value["musiclist"].toList();
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

                    info.m_artistId = value["artistid"].toString();
                    info.m_artistName = ReqKWInterface::makeSongArtist(value["artist"].toString());

                    info.m_albumId = item.m_id;
                    info.m_albumName = TTK::String::charactersReplace(item.m_count);

                    info.m_coverUrl = item.m_coverUrl;
                    info.m_lrcUrl = TTK::Algorithm::mdII(KW_SONG_LRC_URL, false).arg(info.m_songId);
                    info.m_duration = TTKTime::formatDuration(value["duration"].toInt() * TTK_DN_S2MS);
                    info.m_year = value["releasedate"].toString().section(TTK_DEFAULT_STR, 0, 0);
                    info.m_trackNumber = "0";

                    TTK_NETWORK_QUERY_CHECK();
                    ReqKWInterface::parseFromSongProperty(&info, value["formats"].toString());
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



MusicKWQueryArtistAlbumRequest::MusicKWQueryArtistAlbumRequest(QObject *parent)
    : MusicQueryAlbumRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_12;
    m_queryServer = QUERY_KW_INTERFACE;
}

void MusicKWQueryArtistAlbumRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_ARTIST_ALBUM_URL, false).arg(m_queryValue).arg(offset).arg(m_pageSize));
    ReqKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWQueryArtistAlbumRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicPageQueryRequest::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll().replace("'", "\""), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value.contains("albumlist"))
            {
                m_totalSize = value["total"].toInt();

                const QVariantList &datas = value["albumlist"].toList();
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
                    item.m_name = value["name"].toString();
                    item.m_time = value["pub"].toString().replace(TTK_DEFAULT_STR, TTK_DOT);
                    item.m_coverUrl = ReqKWInterface::makeCoverPixmapUrl(value["pic"].toString(), {});
                    Q_EMIT createAlbumItem(item);
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}



MusicKWQueryNewAlbumRequest::MusicKWQueryNewAlbumRequest(QObject *parent)
    : MusicQueryAlbumRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_30;
    m_queryServer = QUERY_KW_INTERFACE;
}

void MusicKWQueryNewAlbumRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_NEW_ALBUM_URL, false).arg(offset).arg(m_pageSize));
    ReqKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWQueryNewAlbumRequest::downloadFinished()
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
            if(value.contains("musiclist"))
            {
                m_totalSize = value["total"].toInt();

                const QVariantList &datas = value["musiclist"].toList();
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
                    item.m_name = value["name"].toString();
                    item.m_nickName = value["artist"].toString();
                    item.m_coverUrl = value["albumpic"].toString().replace("/120/", "/500/");
                    Q_EMIT createAlbumItem(item);
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}
