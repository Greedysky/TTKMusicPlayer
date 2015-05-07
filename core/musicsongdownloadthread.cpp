#include "musicsongdownloadthread.h"
#include <QHostInfo>

MusicSongDownloadThread::MusicSongDownloadThread(const QString &url, QString savePathName,
             QObject *parent):MusicLrcDownLoadThread(url,"",parent)
{
    if(savePathName.isEmpty())
       return;
//    if(QFile::exists(savePathName))
//        savePathName.insert(savePathName.lastIndexOf('.'),"-Copy");
    if(QFile::exists(savePathName))
       QFile::remove(savePathName);
    m_file = new QFile(savePathName,this);
    if( !m_file->exists() || m_file->size() < 4 )
    {
        if( m_file->open(QIODevice::WriteOnly) )
        {
            ///Check the net is online
//            if(QHostInfo::fromName("www.baidu.com").addresses().isEmpty())
//            {
//               deleteAll();
//               return;
//            }

            m_manager = new QNetworkAccessManager(this);
            startRequest(url);
        }
        else
        {
            qDebug() <<"The Song file create failed";
            emit musicDownLoadFinished("The lrc file create failed");
            deleteAll();
        }
    }
}

void MusicSongDownloadThread::startRequest(const QUrl& url)
{
    m_reply = m_manager->get( QNetworkRequest(QUrl(url)));
    connect(m_reply, SIGNAL(finished()), this, SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(replyError(QNetworkReply::NetworkError)) );
    connect(m_reply, SIGNAL(readyRead()),this, SLOT(downLoadReadyRead()));
}

void MusicSongDownloadThread::downLoadFinished()
{
    if(!m_file) return;

    m_file->flush();
    m_file->close();
    QVariant redirectionTarget = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if(m_reply->error())
    {
      m_file->remove();
    }
    else if(!redirectionTarget.isNull())
    {
//      QUrl newUrl = reply->url().resolved(redirectionTarget.toUrl());
      m_reply->deleteLater();
      m_file->open(QIODevice::WriteOnly);
      m_file->resize(0);
      startRequest(m_reply->url().resolved(redirectionTarget.toUrl()));
      return;
    }
    else
    {
        emit musicDownLoadFinished("Music");
        qDebug()<<"Music or pic download has finished!";
        deleteAll();
    }
}

void MusicSongDownloadThread::downLoadReadyRead()
{
    if(m_file)
      m_file->write(m_reply->readAll());
}
