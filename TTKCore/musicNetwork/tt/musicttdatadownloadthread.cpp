#include "musicttdatadownloadthread.h"
#///QJson import
#include "qjson/parser.h"

MusicTTDataDownloadThread::MusicTTDataDownloadThread(const QString &url, const QString &save,
                                                     Download_Type type, QObject *parent)
    : MusicDataDownloadThread(url, save, type, parent)
{
    m_dataReply = nullptr;
    m_dataManager = new QNetworkAccessManager(this);
#ifndef QT_NO_SSL
    connect(m_dataManager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                           SLOT(dataSslErrors(QNetworkReply*,QList<QSslError>)));
    M_LOGGER_INFO(QString("%1 Support ssl: %2").arg(getClassName()).arg(QSslSocket::supportsSsl()));
#endif
}

QString MusicTTDataDownloadThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicTTDataDownloadThread::startToDownload()
{
    m_timer.start(MT_S2MS);
    QNetworkRequest request;
    request.setUrl(m_url);
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_dataReply = m_dataManager->get( request );
    connect(m_dataReply, SIGNAL(finished()), SLOT(dataGetFinished()));
    connect(m_dataReply, SIGNAL(error(QNetworkReply::NetworkError)),
                         SLOT(dataReplyError(QNetworkReply::NetworkError)) );
    connect(m_dataReply, SIGNAL(downloadProgress(qint64, qint64)), SLOT(downloadProgress(qint64, qint64)));
}

void MusicTTDataDownloadThread::deleteAll()
{
    if(m_dataManager)
    {
        m_dataManager->deleteLater();;
        m_dataManager = nullptr;
    }
    if(m_dataReply)
    {
        m_dataReply->deleteLater();
        m_dataReply = nullptr;
    }
    MusicDataDownloadThread::deleteAll();
}

void MusicTTDataDownloadThread::dataGetFinished()
{
    if(!m_dataReply)
    {
        return;
    }

    m_timer.stop();
    if(m_dataReply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_dataReply->readAll();

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok && data.toMap()["code"].toInt() == 1)
        {
            QVariantMap value = data.toMap();
            value = value["data"].toMap();
            m_url = value["singerPic"].toString();

            emit urlHasChanged(m_url);
            MusicDataDownloadThread::startToDownload();
        }
    }

    if(m_dataManager)
    {
        m_dataManager->deleteLater();;
        m_dataManager = nullptr;
    }
    if(m_dataReply)
    {
        m_dataReply->deleteLater();
        m_dataReply = nullptr;
    }
}

void MusicTTDataDownloadThread::dataReplyError(QNetworkReply::NetworkError)
{
    emit downLoadDataChanged("The ttpod data create failed");
    deleteAll();
}

#ifndef QT_NO_SSL
void MusicTTDataDownloadThread::dataSslErrors(QNetworkReply* reply, const QList<QSslError> &errors)
{
    sslErrorsString(reply, errors);
    emit downLoadDataChanged("The ttpod data create failed");
    deleteAll();
}
#endif
