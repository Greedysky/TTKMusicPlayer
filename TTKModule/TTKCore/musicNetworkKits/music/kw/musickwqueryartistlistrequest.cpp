#include "musickwqueryartistlistrequest.h"

MusicKWQueryArtistListRequest::MusicKWQueryArtistListRequest(QObject *parent)
    : MusicQueryArtistListRequest(parent)
{
    m_pageSize = ARTIST_LIST_PAGE_SIZE;
    m_queryServer = QUERY_KW_INTERFACE;
}

void MusicKWQueryArtistListRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(className() << __FUNCTION__ << offset);

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
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWQueryArtistListRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << __FUNCTION__);

    MusicQueryArtistListRequest::downLoadFinished();
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

//    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
