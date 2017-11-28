#include "musicbddiscoverlistthread.h"
#include "musicdownloadbdinterface.h"
#///QJson import
#include "qjson/parser.h"

MusicBDDiscoverListThread::MusicBDDiscoverListThread(QObject *parent)
    : MusicDownLoadDiscoverListThread(parent)
{
    m_pageSize = 100;
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
    QUrl musicUrl = MusicUtils::Algorithm::mdII(BD_SONG_TOPLIST_URL, false).arg(2).arg(m_pageSize).arg(0);
    deleteAll();
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL_1, ALG_UA_KEY, false).toUtf8());
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
    if(!m_reply)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["error_code"].toInt() == 22000 && value.contains("song_list"))
            {
                QVariantList datas = value["song_list"].toList();
                int where = datas.count();
                where = (where > 0) ? qrand()%where : 0;

                int counter = 0;
                foreach(const QVariant &var, datas)
                {
                    if(m_interrupt) return;

                    if((where != counter++) || var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    m_topListInfo = QString("%1 - %2").arg(value["author"].toString())
                                                      .arg(value["title"].toString());
                    break;
                }
            }
        }
    }

    emit downLoadDataChanged(m_topListInfo);
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}
