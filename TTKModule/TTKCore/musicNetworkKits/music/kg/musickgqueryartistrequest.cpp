#include "musickgqueryartistrequest.h"

static constexpr const char *KG_ARTIST_URL = "WnFuNGZxOVZEZEpYekZJOEhhZzFuOENvbURCMFlBNGFlbzJhMUxMY3hSQUg3S3VyMEJRQ1BkVEJqUmJVKytmaW9vTWFUeEtvZjkvVW9HeTZDdTBTanVKeDNNU1B4aC8zSGZaZXRsYnBhNGwyYkd0anFBc2JyRS9Ebks4NlZJRjJGSTBobnlMZklvT3pYVTBHNlhRdXNRPT0=";
static constexpr const char *KG_ARTIST_INFO_URL = "ZVl0R25mTkNsQUVIbnVKaXMzU3pvSXNFOVRqZWh2Nm5RSTFPMHNBTkpLYS9ldjVpRysyT3lYbEQ4bUtKUFNHWGZmVlJsZzBaZmMzTFJDUzdvMEhIQTF6cHVQblVNazNYUUpDN2xQbVpZZVU9";

MusicKGQueryArtistRequest::MusicKGQueryArtistRequest(QObject *parent)
    : MusicQueryArtistRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_50;
    m_queryServer = QUERY_KG_INTERFACE;
}

void MusicKGQueryArtistRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_ARTIST_URL, false).arg(m_queryValue).arg(offset + 1).arg(m_pageSize));
    ReqKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGQueryArtistRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downloadFinished();
    TTK_NETWORK_QUERY_CHECK();
    ReqKGInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    fetchUrlPathSize(&info->m_songProps, info->m_duration);
    MusicQueryArtistRequest::startToQueryResult(info, bitrate);
}

void MusicKGQueryArtistRequest::downloadFinished()
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

                    info.m_artistId = m_queryValue;

                    info.m_albumId = value["album_id"].toString();
                    info.m_albumName = TTK::String::charactersReplace(value["album_name"].toString());

                    info.m_duration = TTKTime::formatDuration(value["duration"].toInt() * TTK_DN_S2MS);
                    info.m_year.clear();
                    info.m_trackNumber = "0";

                    TTK_NETWORK_QUERY_CHECK();
                    ReqKGInterface::parseFromSongAlbumLrc(&info);
                    TTK_NETWORK_QUERY_CHECK();
                    ReqKGInterface::parseFromSongProperty(&info, value);
                    TTK_NETWORK_QUERY_CHECK();

                    if(!m_artistFound)
                    {
                        m_artistFound = true;

                        MusicResultDataItem item;
                        item.m_id = info.m_artistId;
                        item.m_name = info.m_artistName;
                        item.m_coverUrl = info.m_coverUrl;

                        TTK_NETWORK_QUERY_CHECK();
                        queryArtistIntro(&item);
                        TTK_NETWORK_QUERY_CHECK();
                        Q_EMIT createArtistItem(item);
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

void MusicKGQueryArtistRequest::queryArtistIntro(MusicResultDataItem *item) const
{
    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_ARTIST_INFO_URL, false).arg(m_queryValue));
    ReqKGInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    QJsonParseError ok;
    const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
    if(QJsonParseError::NoError == ok.error)
    {
        QVariantMap value = json.toVariant().toMap();
        if(value["errcode"].toInt() == 0)
        {
            value = value["data"].toMap();

            item->m_nickName = value["alias"].toString();
            item->m_description = value["intro"].toString();

            if(item->m_nickName.isEmpty())
            {
                item->m_nickName = item->m_name;
            }
        }
    }
}
