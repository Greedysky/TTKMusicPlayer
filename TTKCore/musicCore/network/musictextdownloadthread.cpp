#include "musictextdownloadthread.h"

#ifdef MUSIC_GREATER_NEW
#   include <QJsonObject>
#   include <QJsonValue>
#   include <QJsonParseError>
#else
#   include <QtScript/QScriptEngine>
#   include <QtScript/QScriptValue>
#endif

MusicTextDownLoadThread::MusicTextDownLoadThread(const QString &url, const QString &save,
                                                 Download_Type type, QObject *parent)
    : MusicDownLoadThreadAbstract(url, save, type, parent)
{

}

QString MusicTextDownLoadThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicTextDownLoadThread::startToDownload()
{
    if( !m_file->exists() || m_file->size() < 4 )
    {
        if( m_file->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text) )
        {
            m_timer.start(MT_S2MS);
            m_manager = new QNetworkAccessManager(this);

            QNetworkRequest request;
            request.setUrl(m_url);
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
            emit downLoadDataChanged("The text file create failed");
            M_LOGGER_ERROR("The text file create failed!");
            deleteAll();
        }
    }
}

void MusicTextDownLoadThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }
    m_timer.stop();

    ///Get all the data obtained by request
    QByteArray bytes = m_reply->readAll();
#ifndef USE_MULTIPLE_QUERY
    if(!bytes.contains("\"code\":2"))
    {
#ifdef MUSIC_GREATER_NEW
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
                QTextStream outstream(&m_file);
                outstream.setCodec("utf-8");
                outstream << jsonObject.take("lrc").toString().remove("\r").toUtf8() << endl;
                m_file->close();
                M_LOGGER_INFO("text download  has finished!");
            }
        }
#else
        QScriptEngine engine;
        QScriptValue sc = engine.evaluate("value=" + QString(bytes));
        if(!sc.property("data").isNull())
        {
            sc = sc.property("data");
            if(!sc.property("lrc").isNull())
            {
                QTextStream outstream(&m_file);
                outstream.setCodec("utf-8");
                outstream << sc.property("lrc").toString().remove("\r").toUtf8() << endl;
                m_file->close();
                M_LOGGER_INFO("text download  has finished!");
            }
        }
#endif
    }
    else
    {
        M_LOGGER_ERROR("text download file error!");
        m_file->remove();
        m_file->close();
    }
#else
    if(!bytes.isEmpty())
    {
        QTextStream outstream(m_file);
        outstream.setCodec("utf-8");
        outstream << QString(bytes).remove("\r").toUtf8() << endl;
        m_file->close();
        M_LOGGER_INFO("text download has finished!");
    }
    else
    {
        M_LOGGER_ERROR("text download file error!");
        m_file->remove();
        m_file->close();
    }
#endif

    emit downLoadDataChanged("Lrc");
    deleteAll();
}
