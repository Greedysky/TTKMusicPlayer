#include "musickwqueryartistlistrequest.h"

static constexpr const char *KW_ARTIST_LIST_URL = "NEtJeVhYZFJ4NE4reDlqU2paYVhpUlZqVWtWWXBnbUVnTGVFRVdkbzBTb2dldXhtdnpaNlhVTHZzSk1yaU0wTHowZTR2dWxiYzg4c2xybmtsNU1FU2w1aFZKcTdsWERvMzVKUVdNSWNQemdUQ1FLMVF3V3RUdE1iVTZmQ1FRK3g=";
static constexpr const char *KW_HOT_ARTIST_LIST_URL = KW_ARTIST_LIST_URL;

MusicKWQueryArtistListRequest::MusicKWQueryArtistListRequest(QObject *parent)
    : MusicQueryArtistListRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_100;
    m_queryServer = QUERY_KW_INTERFACE;
}

void MusicKWQueryArtistListRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QString catId = "0", initial;
    const QStringList &dds = m_queryValue.split(TTK_SPLITER);

    if(dds.count() == 2)
    {
        catId = dds[0];
        if(catId.isEmpty())
        {
            catId = "0";
        }

        const int id = dds[1].toInt();
        if(id > 0 && id <= 26)
        {
            initial = QString("&prefix=%1").arg(TTKStaticCast(char, id + 65 - 1));
        }
    }

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_ARTIST_LIST_URL, false).arg(catId).arg(offset).arg(m_pageSize) + initial);
    ReqKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWQueryArtistListRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicQueryArtistListRequest::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll().replace("'", "\""), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value.contains("artistlist"))
            {
                m_totalSize = value["total"].toInt();

                const QVariantList &datas = value["artistlist"].toList();
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



MusicKWQueryHotArtistListRequest::MusicKWQueryHotArtistListRequest(QObject *parent)
    : MusicQueryArtistListRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_30;
    m_queryServer = QUERY_KW_INTERFACE;
    m_totalSize = 100;
}

void MusicKWQueryHotArtistListRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_HOT_ARTIST_LIST_URL, false).arg(0).arg(offset).arg(m_pageSize));
    ReqKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWQueryHotArtistListRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicAbstractQueryRequest::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll().replace("'", "\""), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value.contains("artistlist"))
            {
                m_totalSize = value["total"].toInt();

                const QVariantList &datas = value["artistlist"].toList();
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
                    item.m_coverUrl = ReqKWInterface::makeArtistPixmapUrl(value["pic"].toString());
                    Q_EMIT createArtistListItem(item);
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}
