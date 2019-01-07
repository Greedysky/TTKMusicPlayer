#include "musicqqtextdownloadthread.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

#define HOST_URL    "ellnUHg0Um83L2x1U29LbWw1UjFtandwRHNIRUxPcnQ="
#define REFER_URL   "YnZJaDZBVEFHSllTWlRualJFblR3U0NkYitRd1N1ZmNKaDZFQUdQVFRKND0="

MusicQQTextDownLoadThread::MusicQQTextDownLoadThread(const QString &url, const QString &save, MusicObject::DownloadType  type, QObject *parent)
    : MusicDownLoadThreadAbstract(url, save, type, parent)
{

}

void MusicQQTextDownLoadThread::startToDownload()
{
    if(m_file && (!m_file->exists() || m_file->size() < 4))
    {
        if(m_file->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        {
            m_timer.start(MT_S2MS);
            m_manager = new QNetworkAccessManager(this);

            QNetworkRequest request;
            request.setUrl(m_url);
            request.setRawHeader("Host", MusicUtils::Algorithm::mdII(HOST_URL, false).toUtf8());
            request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(REFER_URL, false).toUtf8());
#ifndef QT_NO_SSL
            connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
            M_LOGGER_INFO(QString("%1 Support ssl: %2").arg(getClassName()).arg(QSslSocket::supportsSsl()));
            MusicObject::setSslConfiguration(&request);
#endif
            m_reply = m_manager->get(request);
            connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
            connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)) );
            connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), SLOT(downloadProgress(qint64, qint64)));
        }
        else
        {
            emit downLoadDataChanged("The qq text file create failed");
            M_LOGGER_ERROR(QString("%1 file create failed!").arg(getClassName()));
            deleteAll();
        }
    }
}

void MusicQQTextDownLoadThread::downLoadFinished()
{
    if(!m_reply || !m_file)
    {
        deleteAll();
        return;
    }
    m_timer.stop();

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();
        bytes.replace("MusicJsonCallback(", "");
        bytes.chop(1);

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QByteArray lrcData;
            const QVariantMap &value = data.toMap();
            if(value.contains("retcode") && value["retcode"].toInt() == 0)
            {
                lrcData = value["lyric"].toByteArray();
                for(int i=0; i<255; ++i)
                {
                    lrcData.replace(QString("&#%1;").arg(i), QString(QChar(i)).toUtf8());
                }
            }
            QTextStream outstream(m_file);
            outstream.setCodec("utf-8");
            outstream << lrcData << endl;
            m_file->close();
            M_LOGGER_INFO(QString("%1 download has finished!").arg(getClassName()));
        }
        else
        {
            M_LOGGER_ERROR(QString("%1 download file error!").arg(getClassName()));
            m_file->remove();
            m_file->close();
        }
    }

    emit downLoadDataChanged( transferData() );
    deleteAll();
}
