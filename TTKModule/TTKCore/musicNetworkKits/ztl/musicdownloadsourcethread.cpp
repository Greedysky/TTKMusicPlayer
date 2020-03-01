#include "musicdownloadsourcethread.h"

MusicDownloadSourceThread::MusicDownloadSourceThread(QObject *parent)
    : MusicNetworkAbstract(parent)
{

}

MusicDownloadSourceThread::~MusicDownloadSourceThread()
{
    deleteAll();
}

void MusicDownloadSourceThread::startToDownload(const QString &url)
{
    m_manager = new QNetworkAccessManager(this);

    QNetworkRequest request;
    request.setUrl(url);
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    MusicObject::setSslConfiguration(&request);
#endif

    m_reply = m_manager->get( request );
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
}

void MusicDownloadSourceThread::downLoadFinished()
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
            if(m_rawData.isEmpty())
            {
                Q_EMIT downLoadByteDataChanged(m_reply->readAll());
            }
            else
            {
                Q_EMIT downLoadExtDataChanged(m_reply->readAll(), m_rawData);
            }
            deleteAll();
        }
    }
    else
    {
        TTK_LOGGER_ERROR("Download source data error");
        if(m_rawData.isEmpty())
        {
            Q_EMIT downLoadByteDataChanged(QByteArray());
        }
        else
        {
            Q_EMIT downLoadExtDataChanged(QByteArray(), m_rawData);
        }
        deleteAll();
    }
}

void MusicDownloadSourceThread::replyError(QNetworkReply::NetworkError)
{
    TTK_LOGGER_ERROR("Abnormal network connection");
    if(m_rawData.isEmpty())
    {
        Q_EMIT downLoadByteDataChanged(QByteArray());
    }
    else
    {
        Q_EMIT downLoadExtDataChanged(QByteArray(), m_rawData);
    }
    deleteAll();
}

#ifndef QT_NO_SSL
void MusicDownloadSourceThread::sslErrors(QNetworkReply* reply, const QList<QSslError> &errors)
{
    sslErrorsString(reply, errors);
    if(m_rawData.isEmpty())
    {
        Q_EMIT downLoadByteDataChanged(QByteArray());
    }
    else
    {
        Q_EMIT downLoadExtDataChanged(QByteArray(), m_rawData);
    }
    deleteAll();
}
#endif
