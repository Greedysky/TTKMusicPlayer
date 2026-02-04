#include "musickwdiscoverlistrequest.h"
#include "musickwqueryinterface.h"

MusicKWDiscoverListRequest::MusicKWDiscoverListRequest(QObject *parent)
    : MusicDiscoverListRequest(parent)
{

}

void MusicKWDiscoverListRequest::startToSearch()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    deleteAll();
    m_item.clear();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_TOPLIST_URL, false).arg(16).arg(0).arg(TTK_PAGE_SIZE_30));
    ReqKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWDiscoverListRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicDiscoverListRequest::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
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

                    m_item = QString("%1 - %2").arg(value["artist"].toString(), value["name"].toString());
                    break;
                }
            }
        }
    }

    Q_EMIT downloadDataChanged(m_item);
    deleteAll();
}
