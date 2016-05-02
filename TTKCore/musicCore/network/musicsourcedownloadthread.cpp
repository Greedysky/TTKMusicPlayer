#include "musicsourcedownloadthread.h"

MusicSourceDownloadThread::MusicSourceDownloadThread(QObject *parent)
    : QObject(parent)
{
    m_reply = nullptr;
    m_manager = nullptr;
}

MusicSourceDownloadThread::~MusicSourceDownloadThread()
{
    deleteAll();
}

void MusicSourceDownloadThread::deleteAll()
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
    if(m_manager)
    {
        m_manager->deleteLater();
        m_manager = nullptr;
    }
}

void MusicSourceDownloadThread::startToDownload(const QString &url)
{
    m_manager = new QNetworkAccessManager(this);

    QNetworkRequest request;
    request.setUrl(url);
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                       SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    M_LOGGER_INFO(QString("MusicSourceDownloadThread Support ssl: %1").arg(QSslSocket::supportsSsl()));

    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif

    m_reply = m_manager->get( request );
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
}

void MusicSourceDownloadThread::downLoadFinished()
{
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        emit recievedData(m_reply->readAll());
    }
    else
    {
        M_LOGGER_ERROR("Download source data error");
        emit recievedData(QByteArray());
    }
    deleteAll();
}

void MusicSourceDownloadThread::replyError(QNetworkReply::NetworkError)
{
    M_LOGGER_ERROR("Abnormal network connection");
    emit recievedData(QByteArray());
    deleteAll();
}

#ifndef QT_NO_SSL
void MusicSourceDownloadThread::sslErrors(QNetworkReply* reply, const QList<QSslError> &errors)
{
    QString errorString;
    foreach(const QSslError &error, errors)
    {
        if(!errorString.isEmpty())
        {
            errorString += ", ";
        }
        errorString += error.errorString();
    }

    M_LOGGER_ERROR(QString("sslErrors: %1").arg(errorString));
    reply->ignoreSslErrors();
    emit recievedData(QByteArray());
    deleteAll();
}
#endif
