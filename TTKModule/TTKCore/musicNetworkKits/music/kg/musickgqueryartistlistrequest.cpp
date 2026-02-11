#include "musickgqueryartistlistrequest.h"

static constexpr const char *KG_ARTIST_LIST_URL = "ZCt5ZDJpbFdCZHNvUWVHZXJjMDl2L0FZd01mMVZSckkxaVJLcDJXYnZyT0FWNWQyWDByNkxZNXdDWm1yZ3BpNlg4MGtsTVpzTDZNWXdjRFU=";
static constexpr const char *KG_HOT_ARTIST_LIST_URL = "L1VhV3pYU0NUbVBWanR4bUZBMDc5QnNhK3p5bHJORHhOM3RXMlJ3eUVwQTR1MkZ5Yk05Tnh4OHRITjFhWUdDTy96UjBzbE8yMUVFQzJvZUVIQXVBZ1lNVFBEVlZ6bkV3SkdoK1NKUmZXZkE9";

MusicKGQueryArtistListRequest::MusicKGQueryArtistListRequest(QObject *parent)
    : MusicQueryArtistListRequest(parent)
{
    m_pageSize = TTK_HIGH_LEVEL; // TTK_PAGE_SIZE_100
    m_queryServer = QUERY_KG_INTERFACE;
}

void MusicKGQueryArtistListRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = m_pageSize; // 0
    m_pageIndex = offset;

    QString catId = "type=1&sextype=1";
    m_rawData["initial"] = "%E7%83%AD%E9%97%A8";
    const QStringList &dds = m_queryValue.split(TTK_SPLITER);

    if(dds.count() == 2)
    {
        catId = dds[0];
        if(catId.isEmpty())
        {
            catId = "area=1&sex=1";
        }

        const int id = dds[1].toInt();
        if(id > 0 && id <= 26)
        {
            m_rawData["initial"] = QString(TTKStaticCast(char, id + 65 - 1));
        }
    }

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_ARTIST_LIST_URL, false).arg(catId));
    ReqKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGQueryArtistListRequest::startToSearch(const QString &value)
{
    m_queryValue = value;
    m_queryValue.replace("n", "&");
    MusicQueryArtistListRequest::startToSearch(m_queryValue);
}

void MusicKGQueryArtistListRequest::downloadFinished()
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
            if(value["errcode"].toInt() == 0 && value.contains("data"))
            {
                value = value["data"].toMap();

                const QVariantList &datas = value["info"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    if(m_rawData["initial"].toString() == QUrl(value["title"].toString()).toEncoded())
                    {
                        for(const QVariant &sg : value["singer"].toList())
                        {
                            if(sg.isNull())
                            {
                                continue;
                            }

                            value = sg.toMap();
                            TTK_NETWORK_QUERY_CHECK();

                            MusicResultDataItem item;
                            item.m_id = value["singerid"].toString();
                            item.m_name = value["singername"].toString();
                            Q_EMIT createArtistListItem(item);
                        }
                        break;
                    }
                }
            }
        }
    }

//    Q_EMIT downloadDataChanged({});
    deleteAll();
}



MusicKGQueryHotArtistListRequest::MusicKGQueryHotArtistListRequest(QObject *parent)
    : MusicQueryArtistListRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_30;
    m_queryServer = QUERY_KG_INTERFACE;
    m_totalSize = 100;
}

void MusicKGQueryHotArtistListRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_HOT_ARTIST_LIST_URL, false).arg(offset + 1).arg(m_pageSize));
    ReqKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGQueryHotArtistListRequest::downloadFinished()
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
                    item.m_id = value["singerid"].toString();
                    item.m_name = value["singername"].toString();
                    item.m_coverUrl = value["imgurl"].toString().replace("{size}", "500");
                    Q_EMIT createArtistListItem(item);
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}
