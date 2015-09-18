#include "musicsongdownloadthread.h"
#include "musicconnectionpool.h"

MusicSongDownloadThread::MusicSongDownloadThread(const QString &url, const QString &save,
                                                 Download_Type type, QObject *parent)
    : MusicDataDownloadThread(url, save, type, parent)
{
    M_Connection->connectMusicDownload(this);
}

void MusicSongDownloadThread::startRequest(const QUrl &url)
{
    MusicDataDownloadThread::startRequest(url);
    /// only download music data can that show progress
    if(m_downloadType == Download_Music)
    {
        m_createItemTime = QDateTime::currentMSecsSinceEpoch();
        emit createDownloadItem(m_savePathName, m_createItemTime);
        connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)),
                         SLOT(downloadProgress(qint64, qint64)));
    }
}

void MusicSongDownloadThread::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    QString total = QString::number(bytesTotal*1.0/1024/1024);
    total = total.left(total.indexOf(".") + 3) + "M";
    emit downloadProgressChanged(bytesReceived*100.0/ bytesTotal,
                                 total, m_createItemTime);
}
