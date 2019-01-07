#include "musickwartistsimilarthread.h"
#include "musicdownloadkwinterface.h"
#include "musicsemaphoreloop.h"
#///QJson import
#include "qjson/parser.h"

MusicKWArtistSimilarThread::MusicKWArtistSimilarThread(QObject *parent)
    : MusicDownLoadSimilarThread(parent)
{

}

void MusicKWArtistSimilarThread::startToSearch(const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(text));
    deleteAll();

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(KW_AR_SIM_URL, false).arg(getArtistNameById(text));
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KW_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

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
        const QString html(m_reply->readAll());
        QRegExp regx("<li><a href=.*data-src=\"([^\"]+).*<span>(.*)</span></a></li>");
        regx.setMinimal(true);

        int pos = html.indexOf(regx);
        while(pos != -1)
        {
            if(m_interrupt) return;

            MusicResultsItem info;
            info.m_id = getArtistIdName(regx.cap(2));
            info.m_coverUrl = regx.cap(1);
            info.m_name = regx.cap(2);
            info.m_updateTime.clear();
            emit createSimilarItem(info);

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

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(KW_ARTIST_INFO_URL, false).arg(id);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KW_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

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
    const QVariant &data = parser.parse(reply->readAll().replace("'", "\""), &ok);
    if(ok)
    {
        const QVariantMap &value = data.toMap();
        name = value["name"].toString();
    }

    return name;
}

QString MusicKWArtistSimilarThread::getArtistIdName(const QString &name)
{
    if(name.isEmpty() || !m_manager)
    {
        return QString();
    }

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(KW_ARTIST_NINFO_URL, false).arg(name);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KW_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    QNetworkAccessManager manager;
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return QString();
    }

    QJson::Parser parser;
    bool ok;
    const QVariant &data = parser.parse(reply->readAll().replace("'", "\""), &ok);

    QString id;
    if(ok)
    {
        const QVariantMap &value = data.toMap();
        id = value["id"].toString();
    }

    return id;
}
