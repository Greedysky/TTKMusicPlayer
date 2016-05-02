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
    m_reply = m_manager->get(QNetworkRequest(QUrl(url)));
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
