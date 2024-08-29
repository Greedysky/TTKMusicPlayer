#include "musickgqueryartistlistrequest.h"

MusicKGQueryArtistListRequest::MusicKGQueryArtistListRequest(QObject *parent)
    : MusicQueryArtistListRequest(parent)
{
    m_pageSize = TTK_HIGH_LEVEL; // ARTIST_LIST_PAGE_SIZE
    m_queryServer = QUERY_KG_INTERFACE;
}

void MusicKGQueryArtistListRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(className() << __FUNCTION__ << offset);

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
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGQueryArtistListRequest::startToSearch(const QString &value)
{
    m_queryValue = value;
    m_queryValue.replace("n", "&");
    MusicQueryArtistListRequest::startToSearch(m_queryValue);
}

void MusicKGQueryArtistListRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << __FUNCTION__);

    MusicQueryArtistListRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data") && value["errcode"].toInt() == 0)
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

//    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
