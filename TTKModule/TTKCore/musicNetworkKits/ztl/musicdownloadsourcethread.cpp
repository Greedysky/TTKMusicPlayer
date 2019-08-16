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
    M_LOGGER_INFO(QString("%1 Support ssl: %2").arg(getClassName()).arg(QSslSocket::supportsSsl()));
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
                emit downLoadByteDataChanged(m_reply->readAll());
            }
            else
            {
                emit downLoadExtDataChanged(m_reply->readAll(), m_rawData);
            }
            deleteAll();
        }
    }
    else
    {
        M_LOGGER_ERROR("Download source data error");
        if(m_rawData.isEmpty())
        {
            emit downLoadByteDataChanged(QByteArray());
        }
        else
        {
            emit downLoadExtDataChanged(QByteArray(), m_rawData);
        }
        deleteAll();
    }
}

void MusicDownloadSourceThread::replyError(QNetworkReply::NetworkError)
{
    M_LOGGER_ERROR("Abnormal network connection");
    if(m_rawData.isEmpty())
    {
        emit downLoadByteDataChanged(QByteArray());
    }
    else
    {
        emit downLoadExtDataChanged(QByteArray(), m_rawData);
    }
    deleteAll();
}

#ifndef QT_NO_SSL
void MusicDownloadSourceThread::sslErrors(QNetworkReply* reply, const QList<QSslError> &errors)
{
    sslErrorsString(reply, errors);
    if(m_rawData.isEmpty())
    {
        emit downLoadByteDataChanged(QByteArray());
    }
    else
    {
        emit downLoadExtDataChanged(QByteArray(), m_rawData);
    }
    deleteAll();
}
#endif
