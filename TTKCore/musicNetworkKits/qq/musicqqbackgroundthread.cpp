#include "musicqqbackgroundthread.h"
#include "musicdownloadsourcethread.h"
#include "musicdatadownloadthread.h"
#include "musicdownloadqqinterface.h"

#///QJson import
#include "qjson/parser.h"

const QString BIG_ART_URL = "dGJmTlZOK1QvMDJENUxjMDk5UVhBWHVCb001eWtnQ1hKSnhsRWxLczNvRm9FV0kwbHhocTk4aml5SCs1Ym5mQU44SU05c1VZYVFzR2hLTEpGQ0hCNmM1ZUlZVnhnMm92QXNGMFN3PT0=";

MusicQQBackgroundThread::MusicQQBackgroundThread(const QString &name, const QString &save,
                                                 QObject *parent)
    : MusicDownloadBackgroundThread(name, save, parent)
{
    m_manager = new QNetworkAccessManager(this);
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                       SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    M_LOGGER_INFO(QString("%1 Support ssl: %2").arg(getClassName()).arg(QSslSocket::supportsSsl()));
#endif
}

QString MusicQQBackgroundThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicQQBackgroundThread::deleteAll()
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
}

void MusicQQBackgroundThread::startToDownload()
{
    QUrl musicUrl = MusicUtils::Algorithm::mdII(QQ_SONG_SEARCH_URL, false).arg(m_artName).arg(0).arg(50);
    deleteAll();

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadDataFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicQQBackgroundThread::downLoadDataFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    QString songId;
    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();///Get all the data obtained by request
        bytes = bytes.replace("callback(", "");
        bytes.chop(1);

        QJson::Parser parser;
        bool ok;

        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                value = value["song"].toMap();
                QVariantList datas = value["list"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    songId = value["songid"].toString();
                    break;
                }
            }
        }
    }

    downLoadUrl(songId);
}

void MusicQQBackgroundThread::downLoadUrlFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QStringList datas;
        QString text(m_reply->readAll());
        QRegExp regx(QString("<url>([^<]+)</url>"));
        int pos = text.indexOf(regx);
        while(pos != -1)
        {
            datas << regx.cap(0).remove("<url>").remove("</url>").trimmed();
            pos += regx.matchedLength();
            pos = regx.indexIn(text, pos);
        }

        foreach(const QString &url, datas)
        {
            if(m_counter < 5)
            {
                M_LOGGER_ERROR(url);
                MusicDataDownloadThread *down = new MusicDataDownloadThread(url, QString("%1%2%3%4").arg(BACKGROUND_DIR_FULL)
                                        .arg(m_savePath).arg(m_counter++).arg(SKN_FILE),
                                        MusicDownLoadThreadAbstract::Download_BigBG, this);
                connect(down, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished()));
                down->startToDownload();
            }
        }
    }
}

void MusicQQBackgroundThread::downLoadUrl(const QString &id)
{
    QUrl musicUrl = MusicUtils::Algorithm::mdII(BIG_ART_URL, false).arg(id);
    deleteAll();

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadUrlFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}
