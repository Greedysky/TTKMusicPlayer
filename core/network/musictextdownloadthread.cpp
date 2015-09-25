#include "musictextdownloadthread.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>

MusicTextDownLoadThread::MusicTextDownLoadThread(const QString &url, const QString &save,
                                                 Download_Type type, QObject *parent)
    : MusicDownLoadThreadAbstract(url, save, type, parent)
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
            connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
            connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                             SLOT(replyError(QNetworkReply::NetworkError)) );
        }
        else
        {
            emit musicDownLoadFinished("The text file create failed");
            M_LOOGER << "The text file create failed";
            deleteAll();
        }
    }
}

void MusicTextDownLoadThread::downLoadFinished()
{
    if(m_reply == NULL)
    {
        return;
    }
    ///Get all the data obtained by request
    QByteArray bytes = m_reply->readAll();
    if(!bytes.contains("\"code\":2"))
    {
        QJsonParseError jsonError;
        QJsonDocument parseDoucment = QJsonDocument::fromJson(bytes, &jsonError);
        ///Put the data into Json
        if(jsonError.error != QJsonParseError::NoError ||
           !parseDoucment.isObject())
        {
            deleteAll();
            return ;
        }

        QJsonObject jsonObject = parseDoucment.object();
        if(jsonObject.contains("data"))
        {
            jsonObject = jsonObject.take("data").toObject();
            if(jsonObject.contains("lrc"))
            {
                m_file->write(jsonObject.take("lrc").toString().remove("\r").toUtf8());
                m_file->flush();
                m_file->close();
                M_LOOGER << "text download has finished!";
            }
        }
    }
    else
    {
        M_LOOGER << "text download file error!";
        m_file->remove();
        m_file->close();
    }

    emit musicDownLoadFinished("Lrc");
    deleteAll();
}
