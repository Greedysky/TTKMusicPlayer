#include "musicdatasourcerequest.h"

MusicDataSourceRequest::MusicDataSourceRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

void MusicDataSourceRequest::deleteAll()
{
    MusicAbstractNetwork::deleteAll();
    deleteLater();
}

void MusicDataSourceRequest::startToRequest(const QString &url)
{
    QNetworkRequest request;
    request.setUrl(url);
    TTK::setSslConfiguration(&request);
    TTK::makeUserAgentHeader(&request);
    TTK::makeContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicDataSourceRequest::downloadFinished()
{
    MusicAbstractNetwork::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        const QVariant &redirection = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
        if(redirection.isValid())
        {
            const QString &url = redirection.toString();
            MusicAbstractNetwork::deleteAll();
            startToRequest(url);
        }
        else
        {
            Q_EMIT downloadRawDataChanged(m_reply->readAll());
            deleteAll();
        }
    }
    else
    {
        TTK_ERROR_STREAM("Download source data error");
        Q_EMIT downloadRawDataChanged({});
        deleteAll();
    }
}
