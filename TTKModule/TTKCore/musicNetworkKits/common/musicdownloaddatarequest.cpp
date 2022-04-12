#include "musicdownloaddatarequest.h"
#include "musicdownloadmanager.h"

MusicDownloadDataRequest::MusicDownloadDataRequest(const QString &url, const QString &save, MusicObject::DownloadType type, QObject *parent)
    : MusicAbstractDownLoadRequest(url, save, type, parent)
{
    m_createItemTime = -1;
    m_redirection = false;
    m_needUpdate = true;
    m_recordType = MusicObject::RecordNull;
}

void MusicDownloadDataRequest::startToDownload()
{
    if(m_file && (!m_file->exists() || m_file->size() < 4))
    {
        if(m_file->open(QIODevice::WriteOnly))
        {
            startRequest(m_url);
        }
        else
        {
            TTK_LOGGER_ERROR("The data file create failed");
            Q_EMIT downLoadDataChanged("The data file create failed");
            deleteAll();
        }
    }
}

void MusicDownloadDataRequest::setRecordType(MusicObject::RecordType type)
{
    m_recordType = type;
}

void MusicDownloadDataRequest::startRequest(const QString &url)
{
    m_speedTimer.start();

    QNetworkRequest request;
    request.setUrl(url);
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), this, SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
    connect(m_reply, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));
    connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), SLOT(downloadProgress(qint64, qint64)));

    /// only download music data can that show progress
    if(m_downloadType == MusicObject::DownloadMusic && !m_redirection)
    {
        m_createItemTime = MusicTime::timestamp();
        G_DOWNLOAD_MANAGER_PTR->connectMusicDownload(MusicDownLoadPairData(m_createItemTime, this, m_recordType));
        Q_EMIT createDownloadItem(m_savePath, m_createItemTime);
    }
}

void MusicDownloadDataRequest::downLoadFinished()
{
    if(!m_file || !m_reply)
    {
        deleteAll();
        return;
    }

    MusicAbstractDownLoadRequest::downLoadFinished();
    m_redirection = false;
    m_file->flush();
    m_file->close();

    const QVariant &redirection = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if(m_reply->error() != QNetworkReply::NoError)
    {
        m_file->remove();
    }
    else if(redirection.isValid())
    {
        m_redirection = true;
        m_reply->deleteLater();
        m_file->open(QIODevice::WriteOnly);
        m_file->resize(0);
        startRequest(redirection.toString());
        return;
    }
    else
    {
        if(m_needUpdate)
        {
            Q_EMIT downLoadDataChanged(mapCurrentQueryData());
            TTK_LOGGER_INFO("Data download has finished");
        }
    }
    deleteAll();
}

void MusicDownloadDataRequest::handleReadyRead()
{
    if(m_file)
    {
        m_file->write(m_reply->readAll());
    }
}

void MusicDownloadDataRequest::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    MusicAbstractDownLoadRequest::downloadProgress(bytesReceived, bytesTotal);
    /// only download music data or oather type can that show progress
    if(m_downloadType == MusicObject::DownloadMusic || m_downloadType == MusicObject::DownloadOther)
    {
        const QString &total = MusicUtils::Number::sizeByte2Label(bytesTotal);
        Q_EMIT downloadProgressChanged(bytesTotal != 0 ? bytesReceived * 100.0 / bytesTotal : 0, total, m_createItemTime);
    }
}

void MusicDownloadDataRequest::updateDownloadSpeed()
{
    const qint64 speed = m_currentReceived - m_hasReceived;
    const QString &label = MusicUtils::Number::speedByte2Label(speed);
    const qint64 time = (speed != 0) ? (m_totalSize - m_currentReceived) / speed : 0;

    Q_EMIT downloadSpeedLabelChanged(label, time);
    MusicAbstractDownLoadRequest::updateDownloadSpeed();
}
