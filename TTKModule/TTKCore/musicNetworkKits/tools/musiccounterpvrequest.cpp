#include "musiccounterpvrequest.h"
#include "ttkabstractxml.h"

#define QUERY_URL   "QkZ1ek5yL0gxbkV6eGU5bEttakZNT2pyNitIZVZoYlNBYlUxMEp2c2swQWdQODRNZmJmdWVqeGIrK0VsRzNwWUNDZjBkU3J6dmlwQklRRVZ6WVdsa2kwQmRhR0YrbmlZRERWSmlzazNNU21EbWRjS0sxSmtKNXZEekt5MWNwTmgvQ2hBOXo4MWlCajF3c2dXMTdTUXNIQmxCbzdCeC9HUC9MR09Sem80dVZRZmpVeDFkUXU1cVpPMUJIeXJIQTlJRHdTZjM2YWZ6ZWdTYTV1dExnUGd3OG9MekxCczJuS0RVbE1iK21DYkxENEhHQ0JwSHU1bVpuVklOY1Z6YU0walRldUxoL3pHOGpoWEVKOWNST0JZcEZoWWtLQT0="

MusicCounterPVRequest::MusicCounterPVRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

void MusicCounterPVRequest::startRequest()
{
    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(QUERY_URL, false));
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
}

void MusicCounterPVRequest::downLoadFinished()
{
    bool ok = false;

    MusicAbstractNetwork::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        TTKXmlDocument xml;
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
