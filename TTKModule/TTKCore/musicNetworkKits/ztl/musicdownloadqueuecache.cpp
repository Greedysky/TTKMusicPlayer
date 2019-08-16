#include "musicdownloadqueuecache.h"
#include "musicnetworkthread.h"

#include <QStringList>

MusicDownloadQueueCache::MusicDownloadQueueCache(MusicObject::DownloadType  type, QObject *parent)
    : MusicDownloadQueueCache(MusicDownloadQueueData(), type, parent)
{

}

MusicDownloadQueueCache::MusicDownloadQueueCache(const MusicDownloadQueueData &data, MusicObject::DownloadType  type, QObject *parent)
    : MusicDownLoadThreadAbstract(data.m_url, data.m_savePath, type, parent)
{
    m_request = nullptr;
    m_isDownload = false;
    m_isAbort = false;

    m_manager = new QNetworkAccessManager(this);
    m_request = new QNetworkRequest();
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    M_LOGGER_INFO(QString("%1 Support ssl: %2").arg(getClassName()).arg(QSslSocket::supportsSsl()));
    MusicObject::setSslConfiguration(m_request);
#endif

}

MusicDownloadQueueCache::MusicDownloadQueueCache(const MusicDownloadQueueDatas &datas, MusicObject::DownloadType  type, QObject *parent)
    : MusicDownloadQueueCache(MusicDownloadQueueData(), type, parent)
{
    addImageQueue(datas);
}

MusicDownloadQueueCache::~MusicDownloadQueueCache()
{
    if(m_request)
    {
        delete m_request;
        m_request = nullptr;
    }
    deleteAll();
}

void MusicDownloadQueueCache::startToDownload()
{
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

void MusicDownloadQueueCache::clear()
{
    m_imageQueue.clear();
}

void MusicDownloadQueueCache::addImageQueue(const MusicDownloadQueueDatas &datas)
{
    m_imageQueue = datas;
}

void MusicDownloadQueueCache::startOrderImageQueue()
{
    if(!m_imageQueue.isEmpty() && M_NETWORK_PTR->isOnline())
    {
        if(QFile::exists(m_imageQueue.first().m_savePath))
        {
            emit downLoadDataChanged(m_imageQueue.takeFirst().m_savePath);
            startOrderImageQueue();
        }
        else
        {
            startDownload(m_imageQueue.first().m_url);
        }
    }
}

void MusicDownloadQueueCache::startDownload(const QString &url)
{
    m_isDownload = true;
    delete m_file;
    m_file = new QFile( m_imageQueue.first().m_savePath, this);
    if(!m_file->open(QFile::WriteOnly))
    {
        m_file->close();
        delete m_file;
        m_file = nullptr;
        return;
    }

    if(!m_request || !m_manager)
    {
        return;
    }

    m_timer.start(MT_S2MS);
    m_request->setUrl(QUrl(url));
    m_reply = m_manager->get(*m_request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(readyRead()), SLOT(readyReadSlot()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(errorSlot(QNetworkReply::NetworkError)));
}

void MusicDownloadQueueCache::downLoadFinished()
{
    if(m_isAbort || !m_request || !m_manager || !m_reply || !m_file)
    {
        return;
    }
    m_timer.stop();

    m_file->flush();
    m_file->close();
    m_reply->deleteLater();
    m_reply = nullptr;
    m_isDownload = false;
    emit downLoadDataChanged(m_imageQueue.takeFirst().m_savePath);

    startOrderImageQueue();
}

void MusicDownloadQueueCache::readyReadSlot()
{
    if(!m_file || !m_reply)
    {
        return;
    }

    m_file->write(m_reply->readAll());
    m_file->flush();
}

void MusicDownloadQueueCache::errorSlot(QNetworkReply::NetworkError code)
{
    if(!m_file || !m_reply)
    {
        return;
    }
#ifndef TTK_DEBUG
    Q_UNUSED(code);
#endif
    M_LOGGER_ERROR(QString("QNetworkReply::NetworkError : %1 %2").arg(code).arg(m_reply->errorString()));
    m_file->flush();
    if(!m_isAbort)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }

    startToDownload();
}
