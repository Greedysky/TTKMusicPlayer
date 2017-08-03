#include "musicbddiscoverlistthread.h"
#include "musicdownloadbdinterface.h"
#///QJson import
#include "qjson/parser.h"

MusicBDDiscoverListThread::MusicBDDiscoverListThread(QObject *parent)
    : MusicDownLoadDiscoverListThread(parent)
{

}

QString MusicBDDiscoverListThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicBDDiscoverListThread::startToSearch()
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch").arg(getClassName()));
    m_topListInfo.clear();
    QUrl musicUrl = MusicUtils::Algorithm::mdII(BD_SONG_TOPLIST_URL, false);
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
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicBDDiscoverListThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    if(m_reply->error() == QNetworkReply::NoError)
    {
        MusicObject::MIntSet ids;
        while(m_reply->canReadLine())
        {
            QString text = m_reply->readLine();
            QRegExp regx(QString("/song/(\\d+)"));
            int pos = QString(text).indexOf(regx);
            while(pos != -1)
            {
                ids << regx.cap(1).toInt();
                pos += regx.matchedLength();
                pos = regx.indexIn(text, pos);
            }
        }

        if(!ids.isEmpty())
        {
            int id = ids.toList()[qrand()%ids.count()];
            searchTopListInformation(QString::number(id));
            return;
        }
    }

    emit downLoadDataChanged(m_topListInfo);
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}

void MusicBDDiscoverListThread::searchTopListInfoFinished()
{
    M_LOGGER_INFO(QString("%1 searchTopListInfoFinished").arg(getClassName()));
    QNetworkReply *reply = MObject_cast(QNetworkReply*, QObject::sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["errorCode"].toInt() == 22000 && value.contains("data"))
            {
                value = value["data"].toMap();
                QVariantList datas = value["songList"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    m_topListInfo = QString("%1 - %2").arg(value["artistName"].toString())
                                                      .arg(value["songName"].toString());
                }
            }
        }
    }

    emit downLoadDataChanged(m_topListInfo);
    deleteAll();
    M_LOGGER_INFO(QString("%1 searchTopListInfoFinished deleteAll").arg(getClassName()));
}

void MusicBDDiscoverListThread::searchTopListInformation(const QString &id)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 searchTopListInformation %2").arg(getClassName()).arg(id));
    QNetworkRequest request;
    request.setUrl(QUrl(MusicUtils::Algorithm::mdII(BD_SONG_FMINFO_URL, false).arg(id)));
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    QNetworkReply *reply = m_manager->get(request);
    connect(reply, SIGNAL(finished()), SLOT(searchTopListInfoFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}
