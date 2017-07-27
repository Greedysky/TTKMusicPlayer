#include "musicdatatagdownloadthread.h"
#include "musicdownloadsourcethread.h"
#include "musicsemaphoreloop.h"
#include "musicsongtag.h"

#include <QImage>

MusicDataTagDownloadThread::MusicDataTagDownloadThread(const QString &url, const QString &save,
                                                       Download_Type type, QObject *parent)
    : MusicDataDownloadThread(url, save, type, parent)
{
    m_needUpdate = false;
}

QString MusicDataTagDownloadThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDataTagDownloadThread::setTags(const QString &smlUrl, const QString &title, const QString &artist)
{
    m_smallPicUrl = smlUrl;
    m_title = title;
    m_artist = artist;
}

void MusicDataTagDownloadThread::startToDownload()
{
    if( m_file && (!m_file->exists() || m_file->size() < 4) )
    {
        if( m_file->open(QIODevice::WriteOnly) )
        {
            m_manager = new QNetworkAccessManager(this);
#ifndef QT_NO_SSL
            connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                               SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
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
        download->startToDownload(m_smallPicUrl);
        connect(this, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
    }

    emit downLoadDataChanged( transferData() );
    M_LOGGER_INFO("data download has finished!");
}

void MusicDataTagDownloadThread::downLoadFinished(const QByteArray &data)
{
    MusicSongTag tag;
    if(tag.readFile(m_savePathName))
    {
        tag.setTagVersion(3);
        tag.setTitle(m_title);
        tag.setArtist(m_artist);
        tag.setCover(data);
        M_LOGGER_INFO("write tag has finished!");
    }

    emit finished();
}
