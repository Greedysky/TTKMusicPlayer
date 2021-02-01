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

void MusicDownloadTagDataRequest::setSongInfo(const MusicSongInfo &info)
{
    m_musicInfo = info;
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
        download->startToDownload(m_musicInfo.getComment());
        connect(this, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
    }

    Q_EMIT downLoadDataChanged(mapCurrentQueryData());
    TTK_LOGGER_INFO("data download has finished");
}

void MusicDownloadTagDataRequest::downLoadFinished(const QByteArray &data)
{
    MusicSongInfo info;
    if(info.read(m_savePath))
    {
        if(M_SETTING_PTR->value(MusicSettingManager::OtherWriteInfo).toBool())
        {
            info.setTitle(m_musicInfo.getTitle());
            info.setArtist(m_musicInfo.getArtist());
            info.setAlbum(m_musicInfo.getAlbum());
            info.setTrackNum(m_musicInfo.getTrackNum());
            info.setYear(m_musicInfo.getYear());
        }

        if(M_SETTING_PTR->value(MusicSettingManager::OtherWriteAlbumCover).toBool())
        {
            info.setCover(data);
        }
        info.save();
        TTK_LOGGER_INFO("write tag has finished");
    }

    Q_EMIT finished();
}
