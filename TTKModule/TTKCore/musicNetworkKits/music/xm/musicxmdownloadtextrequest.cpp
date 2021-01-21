#include "musicxmdownloadtextrequest.h"
#include "musicstringutils.h"

MusicXMDownLoadTextRequest::MusicXMDownLoadTextRequest(const QString &url, const QString &save, MusicObject::DownloadType  type, QObject *parent)
    : MusicAbstractDownLoadRequest(url, save, type, parent)
{

}

void MusicXMDownLoadTextRequest::startToDownload()
{
    if(m_file && (!m_file->exists() || m_file->size() < 4))
    {
        if(m_file->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        {
            m_speedTimer.start();
            m_manager = new QNetworkAccessManager(this);

            m_lrcType = MusicUtils::String::stringSplitToken(m_url);

            QNetworkRequest request;
            request.setUrl(m_url);
#ifndef QT_NO_SSL
            connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
            MusicObject::setSslConfiguration(&request);
#endif

            m_reply = m_manager->get(request);
            connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
            connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
            connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), SLOT(downloadProgress(qint64, qint64)));
        }
        else
        {
            Q_EMIT downLoadDataChanged("The xiami text file create failed");
            TTK_LOGGER_ERROR(QString("%1 file create failed").arg(getClassName()));
            deleteAll();
        }
    }
}

void MusicXMDownLoadTextRequest::downLoadFinished()
{
    m_speedTimer.stop();

    if(m_reply && m_file && m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();
        if(!bytes.isEmpty())
        {
            if(m_lrcType == "lrc")
            {
                QTextStream outstream(m_file);
                outstream.setCodec("utf-8");
                outstream << QString(bytes).remove("\r").toUtf8();
#if TTK_QT_VERSION_CHECK(5,15,0)
                outstream << Qt::endl;
#else
                outstream << endl;
#endif
            }
            else if(m_lrcType == "trc")
            {
                QTextStream outstream(m_file);
                outstream.setCodec("utf-8");
                QString data = QString(bytes).remove("\r");
                data.remove(QRegExp("<[^>]*>"));
                outstream << data.toUtf8();
#if TTK_QT_VERSION_CHECK(5,15,0)
                outstream << Qt::endl;
#else
                outstream << endl;
#endif
            }
            else if(m_lrcType == "txt")
            {
                m_file->remove();
            }
            m_file->close();
            TTK_LOGGER_INFO(QString("%1 download has finished").arg(getClassName()));
        }
        else
        {
            TTK_LOGGER_ERROR(QString("%1 download file error").arg(getClassName()));
            m_file->remove();
            m_file->close();
        }
    }

    Q_EMIT downLoadDataChanged(mapCurrentQueryData());
    deleteAll();
}
