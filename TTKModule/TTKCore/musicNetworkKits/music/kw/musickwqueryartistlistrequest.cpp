#include "musickwqueryartistlistrequest.h"

MusicKWQueryArtistListRequest::MusicKWQueryArtistListRequest(QObject *parent)
    : MusicQueryArtistListRequest(parent)
{
    m_pageSize = 100;
    m_queryServer = QUERY_KW_INTERFACE;
}

void MusicKWQueryArtistListRequest::startToPage(int offset)
{
    TTK_LOGGER_INFO(QString("%1 startToPage %2").arg(className()).arg(offset));

    deleteAll();
    QString catId = "0", initial;
    const QStringList &dds = m_queryValue.split(TTK_SPLITER);
    if(dds.count() == 2)
    {
        catId = dds[0];
        if(catId.isEmpty())
        {
            catId = "0";
        }

        int mIdx = dds[1].toInt();
        if(mIdx > -1 && mIdx < 26)
        {
            initial = QString("&prefix=%1").arg(TTKStatic_cast(char, mIdx + 65));
        }
    }
    m_totalSize = 0;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KW_ARTIST_LIST_URL, false).arg(catId).arg(offset).arg(m_pageSize) + initial);
    MusicKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicKWQueryArtistListRequest::startToSearch(const QString &value)
{
    m_queryValue = value;
    startToPage(0);
}

void MusicKWQueryArtistListRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    MusicQueryArtistListRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll().replace("'", "\""), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("artistlist"))
            {
                m_totalSize = value["total"].toLongLong();
                const QVariantList &datas = value["artistlist"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultsItem info;
                    info.m_id = value["id"].toString();
                    info.m_name = value["name"].toString();
                    Q_EMIT createArtistListItem(info);
                }
            }
        }
    }

//    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
