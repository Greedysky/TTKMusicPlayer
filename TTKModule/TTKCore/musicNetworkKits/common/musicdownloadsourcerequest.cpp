#include "musicdownloadsourcerequest.h"

MusicDownloadSourceRequest::MusicDownloadSourceRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

MusicDownloadSourceRequest::~MusicDownloadSourceRequest()
{
    deleteAll();
}

void MusicDownloadSourceRequest::startToDownload(const QString &url)
{
    m_manager = new QNetworkAccessManager(this);

    QNetworkRequest request;
    request.setUrl(url);
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    MusicObject::setSslConfiguration(&request);
#endif

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
}

void MusicDownloadSourceRequest::downLoadFinished()
{
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        if(m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute).isValid())
        {
            const QString &newUrl = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();
            deleteAll();
            startToDownload(newUrl); ///redirection target url
        }
        else
        {
            Q_EMIT downLoadRawDataChanged(m_reply->readAll());
            deleteAll();
        }
    }
    else
    {
        TTK_LOGGER_ERROR("Download source data error");
        Q_EMIT downLoadRawDataChanged(QByteArray());
        deleteAll();
    }
}

void MusicDownloadSourceRequest::replyError(QNetworkReply::NetworkError)
{
    TTK_LOGGER_ERROR("Abnormal network connection");
    Q_EMIT downLoadRawDataChanged(QByteArray());
    deleteAll();
}

#ifndef QT_NO_SSL
void MusicDownloadSourceRequest::sslErrors(QNetworkReply* reply, const QList<QSslError> &errors)
{
    sslErrorsString(reply, errors);
    Q_EMIT downLoadRawDataChanged(QByteArray());
    deleteAll();
}
#endif
