#include "musiclrcdownloadthread.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QHostInfo>

MusicLrcDownLoadThread::MusicLrcDownLoadThread(const QString &url, QString savePathName,
             QObject *parent) :QObject(parent),m_reply(NULL),m_manager(NULL)
{
    if(savePathName.isEmpty())
       return;
//    if(QFile::exists(savePathName))
//        savePathName.insert(savePathName.lastIndexOf('.'),"-Copy");
    if(QFile::exists(savePathName))
       QFile::remove(savePathName);
    m_file = new QFile(savePathName);
    if( !m_file->exists() || m_file->size() < 4 )
    {
        if( m_file->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text) )
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
            emit musicDownLoadFinished("The lrc file create failed");
            qDebug() <<"The lrc file create failed";
            deleteAll();
        }
    }
}

void MusicLrcDownLoadThread::startRequest(const QUrl& url)
{
    m_reply = m_manager->get( QNetworkRequest(QUrl(url)));
    connect(m_reply, SIGNAL(finished()), this, SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(replyError(QNetworkReply::NetworkError)) );
}

void MusicLrcDownLoadThread::deleteAll()
{
    if(m_file)
    {
       delete m_file;
       m_file = NULL;
    }
    if(m_manager)
    {
       m_manager->deleteLater();;
       m_manager = NULL;
    }
    if(m_reply)
    {
       m_reply->deleteLater();
       m_reply = NULL;
    }

    this->deleteLater();
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

void MusicLrcDownLoadThread::replyError(QNetworkReply::NetworkError)
{
    emit musicDownLoadFinished("The lrc file create failed");
    deleteAll();
}
