#include "musicdownloadqueuerequest.h"

#include <QStringList>

MusicDownloadQueueRequest::MusicDownloadQueueRequest(TTK::Download type, QObject *parent)
    : MusicDownloadQueueRequest(MusicDownloadQueueData(), type, parent)
{

}

MusicDownloadQueueRequest::MusicDownloadQueueRequest(const MusicDownloadQueueData &data, TTK::Download type, QObject *parent)
    : MusicAbstractDownLoadRequest(data.m_url, data.m_path, type, parent),
      m_isDownload(false),
      m_isAbort(false)
{
    m_request = new QNetworkRequest;
    TTK::setSslConfiguration(m_request);
    TTK::makeContentTypeHeader(m_request);
}

MusicDownloadQueueRequest::MusicDownloadQueueRequest(const MusicDownloadQueueDataList &datas, TTK::Download type, QObject *parent)
    : MusicDownloadQueueRequest(MusicDownloadQueueData(), type, parent)
{
    addQueue(datas);
}

MusicDownloadQueueRequest::~MusicDownloadQueueRequest()
{
    delete m_request;
    m_request = nullptr;
    deleteAll();
}

void MusicDownloadQueueRequest::startToRequest()
{
    if(!m_queue.isEmpty())
    {
        startOrderQueue();
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
    m_queue.clear();
}

void MusicDownloadQueueRequest::addQueue(const MusicDownloadQueueDataList &datas)
{
    m_queue = datas;
}

void MusicDownloadQueueRequest::startOrderQueue()
{
    if(m_queue.isEmpty())
    {
        return;
    }

    if(QFile::exists(m_queue.front().m_path))
    {
        Q_EMIT downLoadDataChanged(m_queue.takeFirst().m_path);
        startOrderQueue();
    }
    else if(G_NETWORK_PTR->isOnline())
    {
        startDownload(m_queue.front().m_url);
    }
}

void MusicDownloadQueueRequest::startDownload(const QString &url)
{
    m_isDownload = true;
    delete m_file;
    m_file = new QFile(m_queue.front().m_path, this);

    if(!m_file->open(QIODevice::WriteOnly))
    {
        m_file->close();
        delete m_file;
        m_file = nullptr;
        return;
    }

    if(!m_request || url.isEmpty())
    {
        return;
    }

    m_speedTimer.start();
    m_request->setUrl(url);
    m_reply = m_manager.get(*m_request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(readyRead()), SLOT(handleReadyRead()));
    QtNetworkErrorConnect(m_reply, this, handleError, TTK_SLOT);
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
    m_isDownload = false;

    MusicAbstractNetwork::deleteAll();
    Q_EMIT downLoadDataChanged(m_queue.takeFirst().m_path);

    startOrderQueue();
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
    TTK_ERROR_STREAM("QNetworkReply::NetworkError:" << code << m_reply->errorString());
    m_file->flush();

    if(!m_isAbort)
    {
        MusicAbstractNetwork::deleteAll();
    }

    startToRequest();
}
