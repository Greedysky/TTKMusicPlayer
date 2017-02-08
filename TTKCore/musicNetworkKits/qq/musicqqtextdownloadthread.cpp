#include "musicqqtextdownloadthread.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicQQTextDownLoadThread::MusicQQTextDownLoadThread(const QString &url, const QString &save,
                                                     Download_Type type, QObject *parent)
    : MusicDownLoadThreadAbstract(url, save, type, parent)
{

}

QString MusicQQTextDownLoadThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicQQTextDownLoadThread::startToDownload()
{
    if( !m_file->exists() || m_file->size() < 4 )
    {
        if( m_file->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text) )
        {
            m_timer.start(MT_S2MS);
            m_manager = new QNetworkAccessManager(this);

            QNetworkRequest request;
            request.setUrl(m_url);
            request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
            request.setRawHeader("Host", "lyric.music.qq.com");
            request.setRawHeader("Referer", "http://lyric.music.qq.com");
#ifndef QT_NO_SSL
            connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                               SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
            M_LOGGER_INFO(QString("%1 Support ssl: %2").arg(getClassName()).arg(QSslSocket::supportsSsl()));

            QSslConfiguration sslConfig = request.sslConfiguration();
            sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
            request.setSslConfiguration(sslConfig);
#endif
            m_reply = m_manager->get( request );
            connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
            connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                             SLOT(replyError(QNetworkReply::NetworkError)) );
            connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)),
                             SLOT(downloadProgress(qint64, qint64)));
        }
        else
        {
            emit downLoadDataChanged("The qq text file create failed");
            M_LOGGER_ERROR("The qq text file create failed!");
            deleteAll();
        }
    }
}

void MusicQQTextDownLoadThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }
    m_timer.stop();

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();
        bytes.replace("jsonp1(", "");
        bytes.chop(1);

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QByteArray lrcData;
            QVariantMap value = data.toMap();
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
            M_LOGGER_INFO("qq text download  has finished!");
        }
        else
        {
            M_LOGGER_ERROR("qq text download file error!");
            m_file->remove();
            m_file->close();
        }
    }

    emit downLoadDataChanged( transferData() );
    deleteAll();
}
