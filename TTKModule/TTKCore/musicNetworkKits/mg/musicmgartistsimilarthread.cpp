#include "musicmgartistsimilarthread.h"
#include "musicdownloadmginterface.h"
#include "musicsemaphoreloop.h"
#///QJson import
#include "qjson/parser.h"

MusicMGArtistSimilarThread::MusicMGArtistSimilarThread(QObject *parent)
    : MusicDownLoadSimilarThread(parent)
{

}

QString MusicMGArtistSimilarThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicMGArtistSimilarThread::startToSearch(const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(text));
    QUrl musicUrl = MusicUtils::Algorithm::mdII(MG_AR_SIM_URL, false).arg(text);
    deleteAll();
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(MG_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicMGArtistSimilarThread::downLoadFinished()
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
        QRegExp regx("<a href=\"/v2/music/artist/(\\d+)\" class=\"tag-list similar-tag\"");
        regx.setMinimal(true);
        int pos = html.indexOf(regx);
        while(pos != -1)
        {
            if(m_interrupt) return;

            MusicPlaylistItem info;
            info.m_id = regx.cap(1);
            info.m_updateTime.clear();
            getArtistInfo(&info);

            emit createSimilarItems(info);

            pos += regx.matchedLength();
            pos = regx.indexIn(html, pos);
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}

void MusicMGArtistSimilarThread::getArtistInfo(MusicPlaylistItem *info)
{
    if(info->m_id.isEmpty() || !m_manager)
    {
        return;
    }

    QUrl musicUrl = MusicUtils::Algorithm::mdII(MG_ARTIST_URL, false).arg(info->m_id);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(MG_UA_URL_1, ALG_UA_KEY, false).toUtf8());
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
        return;
    }

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value["code"].toString() == "000000")
        {
            info->m_name = value["singer"].toString();
            info->m_coverUrl = value["img"].toString();
        }
    }
}

