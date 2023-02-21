#include "musicdownloadcounterpvrequest.h"
#include "ttkabstractxml.h"

MusicDownloadCounterPVRequest::MusicDownloadCounterPVRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

void MusicDownloadCounterPVRequest::startRequest()
{
    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QUERY_URL, false));
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
}

void MusicDownloadCounterPVRequest::downLoadFinished()
{
    bool ok = false;

    MusicAbstractNetwork::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        TTKAbstractXml xml;
        if(xml.fromByteArray(m_reply->readAll()))
        {
            const QStringList &data = xml.readXmlMultiTextByTagName("tspan");
            if(!data.isEmpty())
            {
                ok = true;
                Q_EMIT downLoadDataChanged(data.join(""));
            }
        }
    }

    if(!ok)
    {
        TTK_ERROR_STREAM("Counter PV data error");
        Q_EMIT downLoadDataChanged(TTK_DEFAULT_STR);
    }

    deleteAll();
}
