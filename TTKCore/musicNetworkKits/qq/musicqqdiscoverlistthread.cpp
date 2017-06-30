#include "musicqqdiscoverlistthread.h"
#include "musicdownloadqqinterface.h"
#///QJson import
#include "qjson/parser.h"

MusicQQDiscoverListThread::MusicQQDiscoverListThread(QObject *parent)
    : MusicDownLoadDiscoverListThread(parent)
{

}

QString MusicQQDiscoverListThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicQQDiscoverListThread::startToSearch()
{
    if(!m_manager)
    {
        return;
    }

    m_topListInfo.clear();
    QUrl musicUrl = MusicUtils::Algorithm::mdII(QQ_SONG_TOPLIST_C_URL, false).arg(4);
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

void MusicQQDiscoverListThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 0 && value.contains("songlist"))
            {
                QVariantList datas = value["songlist"].toList();
                int where = datas.count();
                where = (where > 0) ? qrand()%where : 0;

                int counter = 0;
                foreach(const QVariant &var, datas)
                {
                    if((where != counter++) || var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    value = value["data"].toMap();

                    QVariantList artistsArray = value["singer"].toList();
                    foreach(const QVariant &artistValue, artistsArray)
                    {
                        if(artistValue.isNull())
                        {
                            continue;
                        }
                        QVariantMap artistMap = artistValue.toMap();
                        m_topListInfo = artistMap["name"].toString();
                    }

                    m_topListInfo += " - " + value["songname"].toString();
                }
            }
        }
    }

    emit downLoadDataChanged(m_topListInfo);
    deleteAll();
}
