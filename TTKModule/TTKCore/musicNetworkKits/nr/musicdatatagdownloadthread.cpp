#include "musicdatatagdownloadthread.h"
#include "musicdownloadsourcethread.h"
#include "musicsemaphoreloop.h"
#include "musicsettingmanager.h"

#include <QImage>

MusicDataTagDownloadThread::MusicDataTagDownloadThread(const QString &url, const QString &save, MusicObject::DownloadType type, QObject *parent)
    : MusicDataDownloadThread(url, save, type, parent)
{
    m_needUpdate = false;
}

void MusicDataTagDownloadThread::setSongTag(const MusicSongTag &tag)
{
    m_musicTag = tag;
}

void MusicDataTagDownloadThread::startToDownload()
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
            disconnect(m_reply, SIGNAL(finished()), this, SLOT(downLoadFinished()));
            connect(m_reply, SIGNAL(finished()), this, SLOT(downLoadFinished()));
        }
        else
        {
            M_LOGGER_ERROR("The data file create failed");
            emit downLoadDataChanged("The data file create failed");
            deleteAll();
        }
    }
}

void MusicDataTagDownloadThread::downLoadFinished()
{
    bool save = (m_file != nullptr);
    MusicDataDownloadThread::downLoadFinished();

    if(m_redirection)
    {
        return;
    }

    if(save)
    {
        MusicSemaphoreLoop loop;
        MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
        connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
        download->startToDownload(m_musicTag.getComment());
        connect(this, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
    }

    emit downLoadDataChanged( transferData() );
    M_LOGGER_INFO("data download has finished!");
}

void MusicDataTagDownloadThread::downLoadFinished(const QByteArray &data)
{
    MusicSongTag tag;
    if(tag.read(m_savePathName))
    {
        if(M_SETTING_PTR->value(MusicSettingManager::OtherWriteInfoChoiced).toBool())
        {
            tag.setTitle(m_musicTag.getTitle());
            tag.setArtist(m_musicTag.getArtist());
            tag.setAlbum(m_musicTag.getAlbum());
            tag.setTrackNum(m_musicTag.getTrackNum());
            tag.setYear(m_musicTag.getYear());
        }
        if(M_SETTING_PTR->value(MusicSettingManager::OtherWriteAlbumCoverChoiced).toBool())
        {
            tag.setCover(data);
        }
        tag.save();
        M_LOGGER_INFO("write tag has finished!");
    }

    emit finished();
}
