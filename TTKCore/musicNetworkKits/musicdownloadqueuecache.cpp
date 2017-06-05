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
        m_request = nullptr;
    }
    deleteAll();
}

QString MusicDownloadQueueCache::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownloadQueueCache::startToDownload()
{
    m_manager = new QNetworkAccessManager(this);
    m_request = new QNetworkRequest();
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                       SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    M_LOGGER_INFO(QString("%1 Support ssl: %2").arg(getClassName()).arg(QSslSocket::supportsSsl()));

    QSslConfiguration sslConfig = m_request->sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    m_request->setSslConfiguration(sslConfig);
#endif
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
        DownloadQueueData data;
        data.m_url = url[i];
        data.m_savePath = savePath[i];
        m_imageQueue << data;
    }
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

    if(!m_request || !m_manager || !m_reply)
    {
        return;
    }

    m_timer.start(MT_S2MS);
    m_request->setUrl(QUrl(url));
    m_reply = m_manager->get(*m_request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(readyRead()), SLOT(readyReadSlot()));
    connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), SLOT(downloadProgress(qint64, qint64)));
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
#ifndef MUSIC_DEBUG
    Q_UNUSED(code);
#endif
    M_LOGGER_ERROR(QString("QNetworkReply::NetworkError : %1 %2").arg(code).arg(m_reply->errorString()));
    m_file->flush();
    if(!m_isAbort)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
}
