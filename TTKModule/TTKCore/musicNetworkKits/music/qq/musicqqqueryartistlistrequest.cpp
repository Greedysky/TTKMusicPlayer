#include "musicqqqueryartistlistrequest.h"

MusicQQQueryArtistListRequest::MusicQQQueryArtistListRequest(QObject *parent)
    : MusicQueryArtistListRequest(parent)
{
    m_pageSize = 100;
    m_queryServer = QUERY_QQ_INTERFACE;
}

void MusicQQQueryArtistListRequest::startToPage(int offset)
{
    TTK_LOGGER_INFO(QString("%1 startToPage %2").arg(className()).arg(offset));

    deleteAll();
    QString catId = "cn_man_", initial = "all";
    const QStringList &dds = m_queryValue.split(TTK_SPLITER);
    if(dds.count() == 2)
    {
        catId = dds[0];
        if(catId.isEmpty())
        {
            catId = "cn_man_";
        }

        const int mIdx = dds[1].toInt();
        if(mIdx > -1 && mIdx < 26)
        {
            initial = QString(TTKStatic_cast(char, mIdx + 65));
        }
        else if(mIdx >= 26)
        {
            initial = "9";
        }
    }
    catId += initial;
    m_totalSize = 0;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_ARTIST_LIST_URL, false).arg(catId).arg(m_pageSize).arg(offset + 1));
    MusicQQInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicQQQueryArtistListRequest::startToSearch(const QString &value)
{
    m_queryValue = value;
    startToPage(0);
}

void MusicQQQueryArtistListRequest::downLoadFinished()
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
            if(value.contains("data") && value["code"].toInt() == 0)
            {
                value = value["data"].toMap();
                m_totalSize = value["total"].toLongLong();
                const QVariantList &datas = value["list"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultsItem info;
                    info.m_id = value["Fsinger_mid"].toString();
                    info.m_name = value["Fsinger_name"].toString();
                    Q_EMIT createArtistListItem(info);
                }
            }
        }
    }

//    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
