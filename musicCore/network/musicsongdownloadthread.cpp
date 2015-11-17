#include "musicsongdownloadthread.h"
#include "musicconnectionpool.h"
#include "musicsettingmanager.h"

#include <QThread>
#ifdef Q_OS_WIN
#   include <windows.h>
#endif

MusicSongDownloadThread::MusicSongDownloadThread(const QString &url, const QString &save,
                                                 Download_Type type, QObject *parent)
    : MusicDataDownloadThread(url, save, type, parent)
{
    M_CONNECTION->connectMusicDownload(this);
    m_createItemTime = -1;
    m_hasRecevied = -1;
    m_currentRecevied = -1;

    connect(&m_timer, SIGNAL(timeout()), SLOT(updateDownloadSpeed()));
}

void MusicSongDownloadThread::startRequest(const QUrl &url)
{
    MusicDataDownloadThread::startRequest(url);
    /// only download music data can that show progress
    if(m_downloadType == Download_Music)
    {
        m_timer.start(1000);
        m_createItemTime = QDateTime::currentMSecsSinceEpoch();
        emit createDownloadItem(m_savePathName, m_createItemTime);
        connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)),
                         SLOT(downloadProgress(qint64, qint64)));
    }
}

void MusicSongDownloadThread::downLoadFinished()
{
    m_timer.stop();
    MusicDataDownloadThread::downLoadFinished();
}

void MusicSongDownloadThread::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    m_currentRecevied = bytesReceived;
    QString total = QString::number(bytesTotal*1.0/1024/1024);
    total = total.left(total.indexOf(".") + 3) + "M";
    emit downloadProgressChanged(bytesReceived*100.0/ bytesTotal,
                                 total, m_createItemTime);
}

void MusicSongDownloadThread::updateDownloadSpeed()
{
    int delta = m_currentRecevied - m_hasRecevied;
    //////////////////////////////////////
    ///limit speed
    if(M_SETTING->value(MusicSettingManager::DownloadLimitChoiced).toInt() == 0)
    {
        int limitValue = M_SETTING->value(MusicSettingManager::DownloadDLoadLimitChoiced).toInt();
        if(limitValue != 0 && delta > limitValue*1024)
        {
#if defined Q_OS_WIN
#   ifdef MUSIC_QT_5
      QThread::msleep(1000 - limitValue*1024*1000/delta);
#   else
      ::Sleep(1000 - limitValue*1024*1000/delta);
#   endif
#endif
            delta = limitValue*1024;
        }
    }
    //////////////////////////////////////
    m_hasRecevied = m_currentRecevied;
}
