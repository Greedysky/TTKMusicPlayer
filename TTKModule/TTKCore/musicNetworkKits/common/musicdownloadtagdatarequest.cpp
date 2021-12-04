#include "musicdownloadtagdatarequest.h"
#include "musicdownloadsourcerequest.h"
#include "musicsettingmanager.h"

#include <QImage>

MusicDownloadTagDataRequest::MusicDownloadTagDataRequest(const QString &url, const QString &save, MusicObject::DownloadType type, QObject *parent)
    : MusicDownloadDataRequest(url, save, type, parent)
{
    m_needUpdate = false;
}

void MusicDownloadTagDataRequest::setSongMeta(MusicSongMeta &meta)
{
    m_musicMeta = std::move(meta);
}

void MusicDownloadTagDataRequest::startToDownload()
{
    if(m_file && (!m_file->exists() || m_file->size() < 4))
    {
        if(m_file->open(QIODevice::WriteOnly))
        {
            startRequest(m_url);
            disconnect(m_reply, SIGNAL(finished()), this, SLOT(downLoadFinished()));
            connect(m_reply, SIGNAL(finished()), this, SLOT(downLoadFinished()));
        }
        else
        {
            TTK_LOGGER_ERROR("The data file create failed");
            Q_EMIT downLoadDataChanged("The data file create failed");
            deleteAll();
        }
    }
}

void MusicDownloadTagDataRequest::downLoadFinished()
{
    bool save = (m_file != nullptr);
    MusicDownloadDataRequest::downLoadFinished();

    if(m_redirection)
    {
        return;
    }

    if(save)
    {
        MusicSemaphoreLoop loop;
        MusicDownloadSourceRequest *download = new MusicDownloadSourceRequest(this);
        connect(download, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
        download->startToDownload(m_musicMeta.comment());
        connect(this, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
    }

    Q_EMIT downLoadDataChanged(mapCurrentQueryData());
    TTK_LOGGER_INFO("Data download has finished");
}

void MusicDownloadTagDataRequest::downLoadFinished(const QByteArray &bytes)
{
    MusicSongMeta meta;
    if(meta.read(m_savePath))
    {
        if(G_SETTING_PTR->value(MusicSettingManager::OtherWriteFileInfo).toBool())
        {
            meta.setTitle(m_musicMeta.title());
            meta.setArtist(m_musicMeta.artist());
            meta.setAlbum(m_musicMeta.album());
            meta.setTrackNum(m_musicMeta.trackNum());
            meta.setYear(m_musicMeta.year());
        }

        if(G_SETTING_PTR->value(MusicSettingManager::OtherWriteAlbumCover).toBool())
        {
            if(bytes.isEmpty())
            {
                TTK_LOGGER_ERROR("Input byte data is empty");
            }
            else
            {
                meta.setCover(bytes);
            }
        }
        meta.save();
        TTK_LOGGER_INFO("Write tag has finished");
    }

    Q_EMIT finished();
}
