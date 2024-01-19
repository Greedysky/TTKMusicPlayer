#include "musickwdiscoverlistrequest.h"
#include "musickwqueryinterface.h"

MusicKWDiscoverListRequest::MusicKWDiscoverListRequest(QObject *parent)
    : MusicDiscoverListRequest(parent)
{

}

void MusicKWDiscoverListRequest::startToSearch()
{
    TTK_INFO_STREAM(className() << "startToSearch");

    deleteAll();
    m_discoverInfo.clear();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_TOPLIST_URL, false).arg(16).arg(0).arg(TOPLIST_PAGE_SIZE));
    ReqKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWDiscoverListRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << "downLoadFinished");

    MusicDiscoverListRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("musiclist"))
            {
                const QVariantList &datas = value["musiclist"].toList();
                int where = datas.count();
                where = (where > 0) ? TTK::random(where) : 0;

                int counter = 0;
                for(const QVariant &var : qAsConst(datas))
                {
                    if((where != counter++) || var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    m_discoverInfo = QString("%1 - %2").arg(value["artist"].toString(), value["name"].toString());
                    break;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(m_discoverInfo);
    deleteAll();
}
