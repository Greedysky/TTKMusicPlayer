#include "musicdownloadqueuerequest.h"

#include <QStringList>

MusicDownloadQueueRequest::MusicDownloadQueueRequest(MusicObject::DownloadType  type, QObject *parent)
    : MusicDownloadQueueRequest(MusicDownloadQueueData(), type, parent)
{

}

MusicDownloadQueueRequest::MusicDownloadQueueRequest(const MusicDownloadQueueData &data, MusicObject::DownloadType  type, QObject *parent)
    : MusicAbstractDownLoadRequest(data.m_url, data.m_savePath, type, parent)
{
    m_isDownload = false;
    m_isAbort = false;

    m_request = new QNetworkRequest;
    MusicObject::setSslConfiguration(m_request);
}

MusicDownloadQueueRequest::MusicDownloadQueueRequest(const MusicDownloadQueueDataList &datas, MusicObject::DownloadType  type, QObject *parent)
    : MusicDownloadQueueRequest(MusicDownloadQueueData(), type, parent)
{
    addImageQueue(datas);
}

MusicDownloadQueueRequest::~MusicDownloadQueueRequest()
{
    if(m_request)
    {
        delete m_request;
        m_request = nullptr;
    }
    deleteAll();
}

void MusicDownloadQueueRequest::startToDownload()
{
    if(!m_imageQueue.isEmpty())
    {
        startOrderImageQueue();
    }
}

void MusicDownloadQueueRequest::abort()
{
    clear();
    if(m_isDownload && m_reply)
    {
        m_isAbort = true;
        m_reply->abort();
        m_file->close();
        m_file->remove();
        delete m_reply;
        m_reply = nullptr;
        m_isAbort = false;
    }
}

void MusicDownloadQueueRequest::clear()
{
    m_imageQueue.clear();
}

void MusicDownloadQueueRequest::addImageQueue(const MusicDownloadQueueDataList &datas)
{
    m_imageQueue = datas;
}

void MusicDownloadQueueRequest::startOrderImageQueue()
{
    if(!m_imageQueue.isEmpty())
    {
        if(QFile::exists(m_imageQueue.front().m_savePath))
        {
            Q_EMIT downLoadDataChanged(m_imageQueue.takeFirst().m_savePath);
            startOrderImageQueue();
        }
        else if(G_NETWORK_PTR->isOnline())
        {
            startDownload(m_imageQueue.front().m_url);
        }
    }
}

void MusicDownloadQueueRequest::startDownload(const QString &url)
{
    m_isDownload = true;
    delete m_file;
    m_file = new QFile(m_imageQueue.front().m_savePath, this);
    if(!m_file->open(QFile::WriteOnly))
    {
        m_file->close();
        delete m_file;
        m_file = nullptr;
        return;
    }

    if(!m_request)
    {
        return;
    }

    m_speedTimer.start();
    m_request->setUrl(url);
    m_reply = m_manager.get(*m_request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(readyRead()), SLOT(handleReadyRead()));
    QtNetworkErrorConnect(m_reply, this, handleError);
}

void MusicDownloadQueueRequest::downLoadFinished()
{
    if(m_isAbort || !m_request || !m_reply || !m_file)
    {
        return;
    }

    MusicAbstractDownLoadRequest::downLoadFinished();
    m_file->flush();
    m_file->close();
    m_reply->deleteLater();
    m_reply = nullptr;
    m_isDownload = false;
    Q_EMIT downLoadDataChanged(m_imageQueue.takeFirst().m_savePath);

    startOrderImageQueue();
}

void MusicDownloadQueueRequest::handleReadyRead()
{
    if(!m_file || !m_reply)
    {
        return;
    }

    m_file->write(m_reply->readAll());
    m_file->flush();
}

void MusicDownloadQueueRequest::handleError(QNetworkReply::NetworkError code)
{
    if(!m_file || !m_reply)
    {
        return;
    }

#ifndef TTK_DEBUG
    Q_UNUSED(code);
#endif
    TTK_LOGGER_ERROR(QString("QNetworkReply::NetworkError : %1 %2").arg(code).arg(m_reply->errorString()));
    m_file->flush();
    if(!m_isAbort)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }

    startToDownload();
}
