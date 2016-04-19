#include "musicdatadownloadthread.h"
#include "musicconnectionpool.h"

MusicDataDownloadThread::MusicDataDownloadThread(const QString &url, const QString &save,
                                                 Download_Type type, QObject *parent)
    : MusicDownLoadThreadAbstract(url, save, type, parent)
{
    m_createItemTime = -1;
}

void MusicDataDownloadThread::startToDownload()
{
    if( !m_file->exists() || m_file->size() < 4 )
    {
        if( m_file->open(QIODevice::WriteOnly) )
        {
            m_manager = new QNetworkAccessManager(this);
            startRequest(m_url);
        }
        else
        {
            M_LOGGER_ERROR("The data file create failed");
            emit musicDownLoadFinished("The data file create failed");
            deleteAll();
        }
    }
}

void MusicDataDownloadThread::startRequest(const QUrl &url)
{
    m_timer.start(1000);
    m_reply = m_manager->get( QNetworkRequest(url));
    connect(m_reply, SIGNAL(finished()), this, SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                     SLOT(replyError(QNetworkReply::NetworkError)) );
    connect(m_reply, SIGNAL(readyRead()),this, SLOT(downLoadReadyRead()));
    connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), SLOT(downloadProgress(qint64, qint64)));
    /// only download music data can that show progress
    if(m_downloadType == Download_Music)
    {
        M_CONNECTION->connectMusicDownload(this);
        m_createItemTime = QDateTime::currentMSecsSinceEpoch();
        emit createDownloadItem(m_savePathName, m_createItemTime);
    }
}

void MusicDataDownloadThread::downLoadFinished()
{
    if(!m_file)
    {
        return;
    }

    m_timer.stop();
    m_file->flush();
    m_file->close();
    QVariant redirectionTarget = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if(m_reply->error())
    {
        m_file->remove();
    }
    else if(!redirectionTarget.isNull())
    {
        m_reply->deleteLater();
        m_file->open(QIODevice::WriteOnly);
        m_file->resize(0);
        startRequest(m_reply->url().resolved(redirectionTarget.toUrl()));
        return;
    }
    else
    {
        emit musicDownLoadFinished("Data");
        M_LOGGER_INFO("data download has finished!");
        deleteAll();
    }
}

void MusicDataDownloadThread::downLoadReadyRead()
{
    if(m_file)
    {
        m_file->write(m_reply->readAll());
    }
}

void MusicDataDownloadThread::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    MusicDownLoadThreadAbstract::downloadProgress(bytesReceived, bytesTotal);
    /// only download music data can that show progress
    if(m_downloadType == Download_Music)
    {
        QString total = QString::number(bytesTotal*1.0/1024/1024);
        total = total.left(total.indexOf(".") + 3) + "M";
        emit downloadProgressChanged(bytesReceived*100.0/ bytesTotal, total, m_createItemTime);
    }
}
