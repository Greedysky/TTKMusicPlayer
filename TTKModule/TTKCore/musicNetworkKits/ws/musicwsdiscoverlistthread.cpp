#include "musicwsdiscoverlistthread.h"
#include "musicdownloadwsinterface.h"
#include "musicsemaphoreloop.h"
#///QJson import
#include "qjson/parser.h"

MusicWSDiscoverListThread::MusicWSDiscoverListThread(QObject *parent)
    : MusicDownLoadDiscoverListThread(parent)
{

}

QString MusicWSDiscoverListThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicWSDiscoverListThread::startToSearch()
{
    if(!m_manager)
    {
        return;
    }

    getUpdateTime();

    M_LOGGER_INFO(QString("%1 startToSearch").arg(getClassName()));
    QUrl musicUrl = MusicUtils::Algorithm::mdII(WS_SONG_TOPLIST_URL, false).arg("yc").arg(1).arg(100).arg(m_updateTime);
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(WS_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicWSDiscoverListThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll(); ///Get all the data obtained by request

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                QVariantList datas = value["songs"].toList();
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
                    QVariantMap artistsMap = value["user"].toMap();
                    m_topListInfo = artistsMap["NN"].toString();
                    m_topListInfo += " - " + value["SN"].toString();
                    break;
                }
            }
        }
    }

    emit downLoadDataChanged(m_topListInfo);
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}

void MusicWSDiscoverListThread::getUpdateTime()
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 getUpdateTime").arg(getClassName()));
    QUrl musicUrl = MusicUtils::Algorithm::mdII(WS_SONG_TOPLIST_T_URL, false);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(WS_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = m_manager->get(request);
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return;
    }

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value["code"].toInt() == 0 && value.contains("data"))
        {
            value = value["data"].toMap();
            MusicObject::MIntList years;
            foreach(const QString &year, value.keys())
            {
                years << year.toInt();
            }
            qSort(years);

            QVariantList datas = value[QString::number(years.last())].toList();
            foreach(const QVariant &var, datas)
            {
                if(var.isNull())
                {
                    continue;
                }

                m_updateTime = var.toString();
                break;
            }
        }
    }
}
