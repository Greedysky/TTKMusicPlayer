#include "musicdownloadtagdatarequest.h"
#include "musicdownloadsourcerequest.h"
#include "musicsemaphoreloop.h"
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
            m_manager = new QNetworkAccessManager(this);
#ifndef QT_NO_SSL
            connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
#endif
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
        download->startToDownload(m_musicMeta.getComment());
        connect(this, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
    }

    Q_EMIT downLoadDataChanged(mapCurrentQueryData());
    TTK_LOGGER_INFO("data download has finished");
}

void MusicDownloadTagDataRequest::downLoadFinished(const QByteArray &data)
{
    MusicSongMeta meta;
    if(meta.read(m_savePath))
    {
        if(G_SETTING_PTR->value(MusicSettingManager::OtherWriteInfo).toBool())
        {
            meta.setTitle(m_musicMeta.getTitle());
            meta.setArtist(m_musicMeta.getArtist());
            meta.setAlbum(m_musicMeta.getAlbum());
            meta.setTrackNum(m_musicMeta.getTrackNum());
            meta.setYear(m_musicMeta.getYear());
        }

        if(G_SETTING_PTR->value(MusicSettingManager::OtherWriteAlbumCover).toBool())
        {
            meta.setCover(data);
        }
        meta.save();
        TTK_LOGGER_INFO("write tag has finished");
    }

    Q_EMIT finished();
}
