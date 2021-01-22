#include "musicxmdiscoverlistrequest.h"
#include "musicxmqueryinterface.h"

MusicXMDiscoverListRequest::MusicXMDiscoverListRequest(QObject *parent)
    : MusicDiscoverListRequest(parent)
{

}

void MusicXMDiscoverListRequest::startToSearch()
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSearch").arg(getClassName()));

    deleteAll();
    m_toplistInfo.clear();

    QNetworkRequest request;
    TTK_NETWORK_MANAGER_CHECK();
    makeTokenQueryUrl(&request, false,
                      MusicUtils::Algorithm::mdII(XM_TOPLIST_DATA_URL, false).arg(103),
                      MusicUtils::Algorithm::mdII(XM_TOPLIST_URL, false));
    TTK_NETWORK_MANAGER_CHECK();
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicXMDiscoverListRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));

    setNetworkAbort(false);

    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                value = value["data"].toMap();
                value = value["billboard"].toMap();
                const QVariantList &datas = value["items"].toList();
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

                    m_toplistInfo = QString("%1 - %2").arg(value["artistName"].toString()).arg(value["songName"].toString());
                    break;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(m_toplistInfo);
    deleteAll();
}
