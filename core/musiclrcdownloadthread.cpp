#include "musiclrcdownloadthread.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>

MusicLrcDownLoadThread::MusicLrcDownLoadThread(const QString &url, const QString &save,
                                                     QObject *parent)
    : MusicDownLoadThreadAbstract(url, save, parent)
{

}

void MusicLrcDownLoadThread::startToDownload()
{
    if( !m_file->exists() || m_file->size() < 4 )
    {
        if( m_file->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text) )
        {
            m_manager = new QNetworkAccessManager(this);
            m_reply = m_manager->get( QNetworkRequest(QUrl(m_url)));
            connect(m_reply, SIGNAL(finished()), this, SLOT(downLoadFinished()));
            connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                       this, SLOT(replyError(QNetworkReply::NetworkError)) );
        }
        else
        {
            emit musicDownLoadFinished("The lrc file create failed");
            qDebug() <<"The lrc file create failed";
            deleteAll();
        }
    }
}

void MusicLrcDownLoadThread::downLoadFinished()
{
    if(m_reply == NULL) return;

    QString s(m_reply->readAll()) ;
    m_file->write(s.toUtf8());
    m_file->flush();
    m_file->close();

    qDebug()<<"lrc download has finished!";
    emit musicDownLoadFinished("Lrc");
    deleteAll();
}
