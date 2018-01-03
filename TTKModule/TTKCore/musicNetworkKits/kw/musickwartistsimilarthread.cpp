#include "musickwartistsimilarthread.h"
#include "musicdownloadkwinterface.h"
#include "musicsemaphoreloop.h"
#///QJson import
#include "qjson/parser.h"

MusicKWArtistSimilarThread::MusicKWArtistSimilarThread(QObject *parent)
    : MusicDownLoadSimilarThread(parent)
{

}

QString MusicKWArtistSimilarThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicKWArtistSimilarThread::startToSearch(const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(text));
    QUrl musicUrl = MusicUtils::Algorithm::mdII(KW_AR_SIM_URL, false).arg(getArtistNameById(text));
    deleteAll();
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KW_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicKWArtistSimilarThread::downLoadFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QString html(m_reply->readAll());
        QRegExp regx("<li><a href=.*data-src=\"([^\"]+).*<span>(.*)</span></a></li>");
        regx.setMinimal(true);
        int pos = html.indexOf(regx);
        while(pos != -1)
        {
            if(m_interrupt) return;

            MusicPlaylistItem info;
            info.m_id = getArtistIdName(regx.cap(2));
            info.m_coverUrl = regx.cap(1);
            info.m_name = regx.cap(2);
            info.m_updateTime.clear();
            emit createSimilarItems(info);

            pos += regx.matchedLength();
            pos = regx.indexIn(html, pos);
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}

QString MusicKWArtistSimilarThread::getArtistNameById(const QString &id)
{
    QString name;
    if(id.isEmpty() || !m_manager)
    {
        return name;
    }

    QUrl musicUrl = MusicUtils::Algorithm::mdII(KW_ARTIST_INFO_URL, false).arg(id);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KW_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    QNetworkAccessManager manager;
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return name;
    }

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll().replace("'", "\""), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        name = value["name"].toString();
    }

    return name;
}

QString MusicKWArtistSimilarThread::getArtistIdName(const QString &name)
{
    QString id;
    if(name.isEmpty() || !m_manager)
    {
        return id;
    }

    QUrl musicUrl = MusicUtils::Algorithm::mdII(KW_ARTIST_NINFO_URL, false).arg(name);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KW_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    QNetworkAccessManager manager;
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return id;
    }

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll().replace("'", "\""), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        id = value["id"].toString();
    }

    return id;
}
