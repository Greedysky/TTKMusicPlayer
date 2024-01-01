#include "musicdownloadmetadatarequest.h"
#include "musicdownloadqueryfactory.h"
#include "musiccoverrequest.h"

MusicDownloadMetaDataRequest::MusicDownloadMetaDataRequest(const QString &url, const QString &path, TTK::Download type, QObject *parent)
    : MusicDownloadDataRequest(url, path, type, parent)
{
    m_needUpdate = false;
}

void MusicDownloadMetaDataRequest::setSongMeta(MusicSongMeta &meta)
{
    m_songMeta = std::move(meta);
}

void MusicDownloadMetaDataRequest::startRequest()
{
    if(m_file && (!m_file->exists() || m_file->size() < 4))
    {
        if(m_file->open(QIODevice::WriteOnly))
        {
            MusicDownloadDataRequest::startRequest(m_url);
            disconnect(m_reply, SIGNAL(finished()), this, SLOT(downLoadFinished()));
            connect(m_reply, SIGNAL(finished()), this, SLOT(downLoadFinished()));
        }
        else
        {
            TTK_ERROR_STREAM("The data file create failed");
            Q_EMIT downLoadDataChanged("The data file create failed");
            deleteAll();
        }
    }
}

void MusicDownloadMetaDataRequest::downLoadFinished()
{
    bool save = (m_file != nullptr);
    MusicDownloadDataRequest::downLoadFinished();

    if(m_redirection)
    {
        return;
    }

    if(save)
    {
        TTKSemaphoreLoop loop;
        connect(this, SIGNAL(finished()), &loop, SLOT(quit()));

        MusicCoverRequest *d = G_DOWNLOAD_QUERY_PTR->makeCoverRequest(this);
        connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
        d->startRequest(m_songMeta.comment());
        loop.exec();
    }

    Q_EMIT downLoadDataChanged(mapCurrentQueryData());
    TTK_INFO_STREAM("Data download has finished");
}

void MusicDownloadMetaDataRequest::downLoadFinished(const QByteArray &bytes)
{
    MusicSongMeta meta;
    if(meta.read(m_savePath))
    {
        if(G_SETTING_PTR->value(MusicSettingManager::OtherWriteFileInfo).toBool())
        {
            meta.setTitle(m_songMeta.title());
            meta.setArtist(m_songMeta.artist());
            meta.setAlbum(m_songMeta.album());
            meta.setTrackNum(m_songMeta.trackNum());
            meta.setYear(m_songMeta.year());
        }

        if(G_SETTING_PTR->value(MusicSettingManager::OtherWriteAlbumCover).toBool())
        {
            if(bytes.isEmpty())
            {
                TTK_ERROR_STREAM("Input byte data is empty");
            }
            else
            {
                meta.setCover(bytes);
            }
        }
        meta.save();
        TTK_INFO_STREAM("Write tag has finished");
    }

    Q_EMIT finished();
}
