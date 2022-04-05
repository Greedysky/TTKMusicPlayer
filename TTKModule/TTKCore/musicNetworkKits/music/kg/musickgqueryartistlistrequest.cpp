#include "musickgqueryartistlistrequest.h"

MusicKGQueryArtistListRequest::MusicKGQueryArtistListRequest(QObject *parent)
    : MusicQueryArtistListRequest(parent)
{
    m_pageSize = DEFAULT_HIGH_LEVEL;
    m_queryServer = QUERY_KG_INTERFACE;
}

void MusicKGQueryArtistListRequest::startToPage(int offset)
{
    TTK_LOGGER_INFO(QString("%1 startToPage %2").arg(className()).arg(offset));

    deleteAll();
    QString catId = "type=1&sextype=1";
    m_rawData["initial"] = "%E7%83%AD%E9%97%A8";
    const QStringList &dds = m_queryText.split(TTK_SPLITER);
    if(dds.count() == 2)
    {
        catId = dds[0];
        if(catId.isEmpty())
        {
            catId = "area=1&sex=1";
        }

        const int id = dds[1].toInt();
        if(id > -1 && id < 26)
        {
            m_rawData["initial"] = QString(TTKStatic_cast(char, id + 65));
        }
        else if(id >= 26)
        {
            m_rawData["initial"] = "%E5%85%B6%E4%BB%96";
        }
    }
    m_totalSize = DEFAULT_HIGH_LEVEL;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KG_ARTIST_LIST_URL, false).arg(catId));
    MusicKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicKGQueryArtistListRequest::startToSearch(const QString &artistlist)
{
    m_queryText = artistlist;
    m_queryText.replace("n", "&");
    startToPage(0);
}

void MusicKGQueryArtistListRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    MusicQueryArtistListRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
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

                            MusicResultsItem info;
                            info.m_id = QString::number(value["singerid"].toLongLong());
                            info.m_name = value["singername"].toString();
                            Q_EMIT createArtistListItem(info);
                        }
                        break;
                    }
                }
            }
        }
    }

//    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
