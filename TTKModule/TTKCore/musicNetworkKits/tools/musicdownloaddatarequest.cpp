#include "musicdownloaddatarequest.h"
#include "musicdownloadmanager.h"

MusicDownloadDataRequest::MusicDownloadDataRequest(const QString &url, const QString &path, TTK::Download type, QObject *parent)
    : MusicDownloadDataRequest(url, path, type, TTK::Record::Null, parent)
{

}

MusicDownloadDataRequest::MusicDownloadDataRequest(const QString &url, const QString &path, TTK::Download type, TTK::Record record, QObject *parent)
    : MusicAbstractDownLoadRequest(url, path, type, parent),
      m_createTime(-1),
      m_redirection(false),
      m_needUpdate(true),
      m_recordType(record)
{

}

void MusicDownloadDataRequest::startToRequest()
{
    if(!m_file || (m_file->exists() && m_file->size() >= 4) || !m_file->open(QIODevice::WriteOnly) || m_url.isEmpty())
    {
        TTK_ERROR_STREAM("The data file create failed");
        Q_EMIT downLoadDataChanged("The data file create failed");
        deleteAll();
        return;
    }

    startToRequest(m_url);
}

void MusicDownloadDataRequest::startToRequest(const QString &url)
{
    m_speedTimer.start();

    QNetworkRequest request;
    request.setUrl(url);
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), this, SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));
    connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), SLOT(downloadProgress(qint64, qint64)));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);

    /// only download music data can that show progress
    if(m_downloadType == TTK::Download::Music && !m_redirection)
    {
        m_createTime = TTKDateTime::currentTimestamp();
        G_DOWNLOAD_MANAGER_PTR->connectNetworkData(MusicDownLoadPairData(m_createTime, this, m_recordType));
        Q_EMIT createDownloadItem(m_savePath, m_createTime);
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
        MusicAbstractNetwork::deleteAll();
        m_redirection = true;
        m_file->open(QIODevice::WriteOnly);
        m_file->resize(0);
        startToRequest(redirection.toString());
        return;
    }
    else
    {
        if(m_needUpdate)
        {
            Q_EMIT downLoadDataChanged(mapCurrentQueryData());
            TTK_INFO_STREAM("Data download has finished");
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
    /// only download music data or other type can that show progress
    if(m_downloadType == TTK::Download::Music || m_downloadType == TTK::Download::Other)
    {
        const QString &total = TTK::Number::sizeByteToLabel(bytesTotal);
        Q_EMIT downloadProgressChanged(bytesTotal != 0 ? bytesReceived * 100.0 / bytesTotal : 0, total, m_createTime);
    }
}

void MusicDownloadDataRequest::updateDownloadSpeed()
{
    const qint64 speed = m_currentReceived - m_hasReceived;
    const QString &label = TTK::Number::speedByteToLabel(speed);
    const qint64 time = (speed != 0) ? (m_totalSize - m_currentReceived) / speed : 0;

    Q_EMIT downloadSpeedLabelChanged(label, time);
    MusicAbstractDownLoadRequest::updateDownloadSpeed();
}
