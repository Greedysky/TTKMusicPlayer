#include "musictextdownloadthread.h"

MusicTextDownLoadThread::MusicTextDownLoadThread(const QString &url, const QString &save,
                                                 QObject *parent)
    : MusicDownLoadThreadAbstract(url, save, parent)
{

}

void MusicTextDownLoadThread::startToDownload()
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
            emit musicDownLoadFinished("The text file create failed");
            qDebug() <<"The text file create failed";
            deleteAll();
        }
    }
}

void MusicTextDownLoadThread::downLoadFinished()
{
    if(m_reply == NULL) return;

    QString s(m_reply->readAll());

    if(!s.contains("\"code\":2"))
    {
        if(s.contains("\"lrc\":"))
        {
            s.replace("\\r",""); //remove the return line
            s.replace("\\n","\n"); //ttop api
        }

        m_file->write(s.toUtf8());
        m_file->flush();
        m_file->close();
        qDebug()<<"text download has finished!";
    }
    else
    {
        qDebug()<<"text download file error!";
        m_file->remove();
        m_file->close();
    }

    emit musicDownLoadFinished("Lrc");
    deleteAll();
}
