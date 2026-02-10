#include "musicwyqueryartistlistrequest.h"

static constexpr const char *WY_ARTIST_LIST_URL = "dkxWWG8wQTVVa0xkY1dydjlwTWtNNGtSb3RBWWZqbURVZGtCWHMraFlSNVVRWG9rSnEwSzI4M3o4SDg9";
static constexpr const char *WY_ARTIST_LIST_DATA_URL = "eWJCN095NWdwcXlZRkpIN1luQlVQV21vZXVxSytrUnIyUDFabFBDWUhvd2N0bkVhbkZyYVU3OXNjR3hJOVF5cUUzWDVQcFQ2RDh1cE1mRG52ZExFVTF0UEJDb09KdUkyWXV4T3p3PT0=";
static constexpr const char *WY_HOT_ARTIST_LIST_URL = "c1l3a204QlJmWnJ0WVhxOTZlNU9RdWJVeGZRWDIvaU83RWxWTTIrRW84aExRajVWWEZXMnJmZlowRFpmWFdEV2NoM29IQT09";
static constexpr const char *WY_HOT_ARTIST_LIST_DATA_URL = "TG9pYmlKeWRiaG45TFlHalhUNWtzYUxDUEZZL0VVdGJEOFlTTkFuaWdmb3hDempSRUMwNkJmSkdpeFU9";

MusicWYQueryArtistListRequest::MusicWYQueryArtistListRequest(QObject *parent)
    : MusicQueryArtistListRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_100;
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicWYQueryArtistListRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = m_pageSize; // 0
    m_pageIndex = offset;

    QString catId = "1001", initial = "-1";
    const QStringList &dds = m_queryValue.split(TTK_SPLITER);

    if(dds.count() == 2)
    {
        catId = dds[0];
        if(catId.isEmpty())
        {
            catId = "1001";
        }

        int id = dds[1].toInt();
        if(id <= 0)
        {
            id = -1;
        }
        else if(id > 0 && id <= 26)
        {
            id += 65 - 1;
        }
        else if(id > 26)
        {
            id = 0;
        }

        initial = QString::number(id);
    }

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_ARTIST_LIST_URL, false),
                      TTK::Algorithm::mdII(WY_ARTIST_LIST_DATA_URL, false).arg(catId, initial).arg(m_pageSize * offset).arg(m_pageSize));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYQueryArtistListRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicQueryArtistListRequest::downloadFinished();
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
                    item.m_id = value["id"].toString();
                    item.m_name = value["name"].toString();
                    Q_EMIT createArtistListItem(item);
                }
            }
        }
    }

//    Q_EMIT downloadDataChanged({});
    deleteAll();
}



MusicWYQueryHotArtistListRequest::MusicWYQueryHotArtistListRequest(QObject *parent)
    : MusicQueryArtistListRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_30;
    m_queryServer = QUERY_WY_INTERFACE;
    m_totalSize = 100;
}

void MusicWYQueryHotArtistListRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_pageIndex = offset;

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                       TTK::Algorithm::mdII(WY_HOT_ARTIST_LIST_URL, false),
                       TTK::Algorithm::mdII(WY_HOT_ARTIST_LIST_DATA_URL, false).arg(m_pageSize * offset).arg(m_pageSize));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYQueryHotArtistListRequest::downloadFinished()
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
                    Q_EMIT createArtistListItem(item);
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}
