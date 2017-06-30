#include "musickgdiscoverlistthread.h"
#include "musicdownloadkginterface.h"
#///QJson import
#include "qjson/parser.h"

MusicKGDiscoverListThread::MusicKGDiscoverListThread(QObject *parent)
    : MusicDownLoadDiscoverListThread(parent)
{

}

QString MusicKGDiscoverListThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicKGDiscoverListThread::startToSearch()
{
    if(!m_manager)
    {
        return;
    }

    m_topListInfo.clear();
    QUrl musicUrl = MusicUtils::Algorithm::mdII(KG_SONG_TOPLIST_URL, false);
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

void MusicKGDiscoverListThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll(); ///Get all the data obtained by request
        bytes = QString(bytes).split("global.features = ").back().split("];").front().toUtf8() + "]";

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantList datas = data.toList();
            int where = datas.count();
            where = (where > 0) ? qrand()%where : 0;

            int counter = 0;
            foreach(const QVariant &var, datas)
            {
                if((where != counter++) || var.isNull())
                {
                    continue;
                }

                QVariantMap value = var.toMap();
                m_topListInfo = value["FileName"].toString();
            }
        }
    }

    emit downLoadDataChanged(m_topListInfo);
    deleteAll();
}
