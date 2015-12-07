#include "musicdownloadqueuecache.h"
#include "musicnetworkthread.h"

#include <QStringList>

MusicDownloadQueueCache::MusicDownloadQueueCache(const QString &url,
                            const QString &save, Download_Type type, QObject *parent)
    : MusicDownLoadThreadAbstract(url, save, type, parent)
{
    m_request = nullptr;
    m_isDownload = false;
    m_isAbort = false;
}

MusicDownloadQueueCache::MusicDownloadQueueCache(const QStringList &url,
                        const QStringList &save, Download_Type type, QObject *parent)
    : MusicDownLoadThreadAbstract(QString(), QString(), type, parent)
{
    m_request = nullptr;
    m_isDownload = false;
    m_isAbort = false;
    addImageQueue(url, save);
}

MusicDownloadQueueCache::~MusicDownloadQueueCache()
{
    if(m_request)
    {
        delete m_request;
    }
    deleteAll();
}

void MusicDownloadQueueCache::startToDownload()
{
    m_manager = new QNetworkAccessManager(this);
    m_request = new QNetworkRequest();
    if(!m_imageQueue.isEmpty())
    {
        startOrderImageQueue();
    }
}

void MusicDownloadQueueCache::abort()
{
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

void MusicDownloadQueueCache::addImageQueue(const QStringList &url,
                                            const QStringList &savePath)
{
    m_imageQueue.clear();
    for(int i=0; i<url.count(); ++i)
    {
        DownloadData data;
        data.url = url[i];
        data.savePath = savePath[i];
        m_imageQueue << data;
    }
}

void MusicDownloadQueueCache::startOrderImageQueue()
{
    if(!m_imageQueue.isEmpty() && M_NETWORK->isOnline())
    {
        if(QFile::exists(m_imageQueue.first().savePath))
        {
            emit musicDownLoadFinished(m_imageQueue.takeFirst().savePath);
            startOrderImageQueue();
        }
        else
        {
            startDownload(m_imageQueue.first().url);
        }
    }
}

void MusicDownloadQueueCache::startDownload(const QString &url)
{
    m_isDownload = true;
    delete m_file;
    m_file = new QFile( m_imageQueue.first().savePath, this);
    if(!m_file->open(QFile::WriteOnly))
    {
        m_file->close();
        delete m_file;
        m_file = nullptr;
        return;
    }
    m_request->setUrl(QUrl(url));
    m_reply = m_manager->get(*m_request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(readyRead()), SLOT(readyReadSlot()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                     SLOT(errorSlot(QNetworkReply::NetworkError)));
}

void MusicDownloadQueueCache::downLoadFinished()
{
    if(m_isAbort)
    {
        return;
    }
    m_file->flush();
    m_file->close();
    m_reply->deleteLater();
    m_reply = nullptr;
    m_isDownload = false;
    emit musicDownLoadFinished(m_imageQueue.takeFirst().savePath);

    startOrderImageQueue();
}

void MusicDownloadQueueCache::readyReadSlot()
{
    m_file->write(m_reply->readAll());
    m_file->flush();
}

void MusicDownloadQueueCache::errorSlot(QNetworkReply::NetworkError code)
{
    M_LOGGER << "QNetworkReply::NetworkError : " << code <<m_reply->errorString() << LOG_END;
    m_file->flush();
    if(!m_isAbort)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
}
