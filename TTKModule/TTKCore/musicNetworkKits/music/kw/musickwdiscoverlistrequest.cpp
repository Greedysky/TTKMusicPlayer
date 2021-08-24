#include "musickwdiscoverlistrequest.h"
#include "musickwqueryinterface.h"

MusicKWDiscoverListRequest::MusicKWDiscoverListRequest(QObject *parent)
    : MusicDiscoverListRequest(parent)
{

}

void MusicKWDiscoverListRequest::startToSearch()
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSearch").arg(getClassName()));

    deleteAll();
    m_toplistInfo.clear();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KW_TOPLIST_URL, false).arg(16));
    MusicKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicKWDiscoverListRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));

    MusicDiscoverListRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("musiclist"))
            {
                const QVariantList &datas = value["musiclist"].toList();
                int where = datas.count();
                where = (where > 0) ? MusicTime::random(where) : 0;

                int counter = 0;
                for(const QVariant &var : qAsConst(datas))
                {
                    if((where != counter++) || var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    m_toplistInfo = QString("%1 - %2").arg(value["artist"].toString()).arg(value["name"].toString());
                    break;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(m_toplistInfo);
    deleteAll();
}
