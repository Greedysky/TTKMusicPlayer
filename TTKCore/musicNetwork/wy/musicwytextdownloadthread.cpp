#include "musicwytextdownloadthread.h"

#ifdef MUSIC_GREATER_NEW
#   include <QJsonObject>
#   include <QJsonValue>
#   include <QJsonParseError>
#else
#   ///QJson import
#   include "qjson/parser.h"
#endif

MusicWYTextDownLoadThread::MusicWYTextDownLoadThread(const QString &url, const QString &save,
                                                     Download_Type type, QObject *parent)
    : MusicDownLoadThreadAbstract(url, save, type, parent)
{

}

QString MusicWYTextDownLoadThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicWYTextDownLoadThread::startToDownload()
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
            emit downLoadDataChanged("The wangyi text file create failed");
            M_LOGGER_ERROR("The wangyi text file create failed!");
            deleteAll();
        }
    }
}

void MusicWYTextDownLoadThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }
    m_timer.stop();

    ///Get all the data obtained by request
    QByteArray bytes = m_reply->readAll();
#ifdef MUSIC_GREATER_NEW
    QJsonParseError jsonError;
    QJsonDocument parseDoucment = QJsonDocument::fromJson(bytes, &jsonError);
    ///Put the data into Json
    if(jsonError.error != QJsonParseError::NoError ||
       !parseDoucment.isObject())
    {
        emit downLoadDataChanged("Lrc");
        deleteAll();
        return ;
    }

    QJsonObject jsonObject = parseDoucment.object();
    if(jsonObject.contains("code") && jsonObject.value("code").toInt() == 200)
    {
        jsonObject = jsonObject.take("lrc").toObject();
        if(!jsonObject.empty())
        {
            QString data = jsonObject.value("lyric").toString();
            QTextStream outstream(m_file);
            outstream.setCodec("utf-8");
            outstream << data.toUtf8() << endl;
            m_file->close();
            M_LOGGER_INFO("wangyi text download  has finished!");
        }
    }
#else
    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("code") && value["code"].toInt() == 200)
        {
            value = value["lrc"].toMap();
            if(!value.isEmpty())
            {
                QString data = value["lyric"].toString();
                QTextStream outstream(m_file);
                outstream.setCodec("utf-8");
                outstream << data.toUtf8() << endl;
                m_file->close();
                M_LOGGER_INFO("wangyi text download  has finished!");
            }
        }
    }
#endif

    emit downLoadDataChanged("Lrc");
    deleteAll();
}
