#include "musicbdartistsimilarthread.h"
#include "musicdownloadbdinterface.h"
#///QJson import
#include "qjson/parser.h"

MusicBDArtistSimilarThread::MusicBDArtistSimilarThread(QObject *parent)
    : MusicDownLoadSimilarThread(parent)
{

}

QString MusicBDArtistSimilarThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicBDArtistSimilarThread::startToSearch(const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(text));
    QUrl musicUrl = MusicUtils::Algorithm::mdII(BD_AR_SIM_URL, false).arg(text);
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

void MusicBDArtistSimilarThread::downLoadFinished()
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
        QRegExp regx("<a href=\"/artist/(\\d+).*img class=\"avatar.*src=\"([^\"]+).*title=\"([^\"]+).*</a>");
        regx.setMinimal(true);
        int pos = html.indexOf(regx);
        while(pos != -1)
        {
            if(m_interrupt) return;

            MusicPlaylistItem info;
            info.m_id = regx.cap(1);
            info.m_coverUrl = regx.cap(2).remove("\t").remove("\n").remove("@s_0,w_120");
            info.m_name = regx.cap(3);
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
