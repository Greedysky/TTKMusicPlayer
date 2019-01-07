#include "musicdatadownloadthread.h"
#include "musicdownloadmanager.h"
#include "musicnumberutils.h"
#include "musictime.h"

MusicDataDownloadThread::MusicDataDownloadThread(const QString &url, const QString &save, MusicObject::DownloadType type, QObject *parent)
    : MusicDownLoadThreadAbstract(url, save, type, parent)
{
    m_createItemTime = -1;
    m_redirection = false;
    m_needUpdate = true;
    m_recordType = MusicObject::RecordNull;
}

void MusicDataDownloadThread::startToDownload()
{
    if(m_file && (!m_file->exists() || m_file->size() < 4))
    {
        if(m_file->open(QIODevice::WriteOnly))
        {
            m_manager = new QNetworkAccessManager(this);
#ifndef QT_NO_SSL
            connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
            M_LOGGER_INFO(QString("%1 Support ssl: %2").arg(getClassName()).arg(QSslSocket::supportsSsl()));
#endif
            startRequest(m_url);
        }
        else
        {
            M_LOGGER_ERROR("The data file create failed");
            emit downLoadDataChanged("The data file create failed");
            deleteAll();
        }
    }
}

void MusicDataDownloadThread::setRecordType(MusicObject::RecordType type)
{
    m_recordType = type;
}

void MusicDataDownloadThread::startRequest(const QUrl &url)
{
    if(!m_manager)
    {
        return;
    }

    m_timer.start(MT_S2MS);

    QNetworkRequest request;
    request.setUrl(url);
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), this, SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)) );
    connect(m_reply, SIGNAL(readyRead()),this, SLOT(downLoadReadyRead()));
    connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), SLOT(downloadProgress(qint64, qint64)));
    /// only download music data can that show progress
    if(m_downloadType == MusicObject::DownloadMusic && !m_redirection)
    {
        m_createItemTime = MusicTime::timeStamp();
        M_DOWNLOAD_MANAGER_PTR->connectMusicDownload(MusicDownLoadPair(m_createItemTime, this, m_recordType));
        emit createDownloadItem(m_savePathName, m_createItemTime);
    }
}

void MusicDataDownloadThread::downLoadFinished()
{
    if(!m_file || !m_reply)
    {
        deleteAll();
        return;
    }

    m_redirection = false;
    m_timer.stop();
    m_file->flush();
    m_file->close();

    const QVariant &redirectionTarget = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if(m_reply->error() != QNetworkReply::NoError)
    {
        m_file->remove();
    }
    else if(!redirectionTarget.isNull())
    {
        m_redirection = true;
        m_reply->deleteLater();
        m_file->open(QIODevice::WriteOnly);
        m_file->resize(0);
        startRequest(m_reply->url().resolved(redirectionTarget.toUrl()));
        return;
    }
    else
    {
        if(m_needUpdate)
        {
            emit downLoadDataChanged( transferData() );
            M_LOGGER_INFO("data download has finished!");
        }
    }
    deleteAll();
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
    /// only download music data or oather type can that show progress
    if(m_downloadType == MusicObject::DownloadMusic || m_downloadType == MusicObject::DownloadOther)
    {
        const QString &total = MusicUtils::Number::size2Label(bytesTotal);
        emit downloadProgressChanged(bytesTotal != 0 ? bytesReceived*100.0/bytesTotal : 0, total, m_createItemTime);
    }
}

void MusicDataDownloadThread::updateDownloadSpeed()
{
    const qint64 speed = m_currentReceived - m_hasReceived;
    const QString &label = MusicUtils::Number::speed2Label(speed);
    const qint64 time = (speed != 0) ? (m_totalSize - m_currentReceived)/speed : 0;

    emit downloadSpeedLabelChanged(label, time);
    MusicDownLoadThreadAbstract::updateDownloadSpeed();
}
