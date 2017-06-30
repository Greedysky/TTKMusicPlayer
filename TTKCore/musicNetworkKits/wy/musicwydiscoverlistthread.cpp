#include "musicwydiscoverlistthread.h"
#include "musicdownloadwyinterface.h"
#///QJson import
#include "qjson/parser.h"

MusicWYDiscoverListThread::MusicWYDiscoverListThread(QObject *parent)
    : MusicDownLoadDiscoverListThread(parent)
{

}

QString MusicWYDiscoverListThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicWYDiscoverListThread::startToSearch()
{
    if(!m_manager)
    {
        return;
    }

    m_topListInfo.clear();
    QUrl musicUrl = MusicUtils::Algorithm::mdII(WY_SONG_TOPLIST_URL, false);
    deleteAll();

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Origin", MusicUtils::Algorithm::mdII(WY_BASE_URL, false).toUtf8());
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(WY_BASE_URL, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicWYDiscoverListThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }

    if(m_reply->error() == QNetworkReply::NoError)
    {
        MusicObject::MIntSet ids;
        while(m_reply->canReadLine())
        {
            QString text = m_reply->readLine();
            QRegExp regx(QString("/song\\?id=(\\d+)"));
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

    emit downLoadDataChanged(QString());
    deleteAll();
}

void MusicWYDiscoverListThread::searchTopListInfoFinished()
{
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
            if(value.contains("code") && value.contains("songs") && value["code"].toInt() == 200)
            {
                QVariantList datas = value["songs"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    QVariantList artistsArray = value["artists"].toList();
                    foreach(const QVariant &artistValue, artistsArray)
                    {
                        if(artistValue.isNull())
                        {
                            continue;
                        }
                        QVariantMap artistMap = artistValue.toMap();
                        m_topListInfo = artistMap["name"].toString();
                    }

                    m_topListInfo += " - " + value["name"].toString();
                }
            }
        }
    }

    emit downLoadDataChanged(m_topListInfo);
    deleteAll();
}

void MusicWYDiscoverListThread::searchTopListInformation(const QString &id)
{
    if(!m_manager)
    {
        return;
    }

    QNetworkRequest request;
    request.setUrl(QUrl(MusicUtils::Algorithm::mdII(WY_SONG_URL, false).arg(id)));
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Origin", MusicUtils::Algorithm::mdII(WY_BASE_URL, false).toUtf8());
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(WY_BASE_URL, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    QNetworkReply *reply = m_manager->get(request);
    connect(reply, SIGNAL(finished()), SLOT(searchTopListInfoFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}
