#include "musicradiosongsthread.h"
#include "musicnumberutils.h"
#///QJson import
#include "qjson/parser.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkCookieJar>

MusicRadioSongsThread::MusicRadioSongsThread(QObject *parent, QNetworkCookieJar *cookie)
    : MusicRadioThreadAbstract(parent, cookie)
{

}

MusicRadioSongsThread::~MusicRadioSongsThread()
{
    deleteAll();
}

void MusicRadioSongsThread::startToDownload(const QString &id)
{
    m_songInfo = MusicObject::MusicSongInformation();
    m_manager = new QNetworkAccessManager(this);

    TTK_LOGGER_INFO(id);

    QNetworkRequest request;
//    request.setUrl(QUrl("https://api.douban.com/v2/fm/playlist?channel=" + id + "&kbps=128&app_name=radio_website&version=100&type=n"));
    request.setUrl(QUrl("https://api.douban.com/v2/fm/playlist?channel=" + id + "&kbps=128&app_name=radio_website&version=100&type=s"));
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    MusicObject::setSslConfiguration(&request);
#endif
    if(m_cookJar)
    {
        m_manager->setCookieJar(m_cookJar);
        m_cookJar->setParent(nullptr);
    }

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicRadioSongsThread::downLoadFinished()
{
    if(!m_reply)
    {
        deleteAll();
        return;
    }

    if(m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            const QVariantList &songLists = value["song"].toList();
            foreach(const QVariant &var, songLists)
            {
                value = var.toMap();
                if(value.isEmpty() || value["url"].toString().isEmpty())
                {
                    continue;
                }

                MusicObject::MusicSongAttribute attr;
                attr.m_url = value["url"].toString();
                attr.m_bitrate = value["kbps"].toInt();
                attr.m_format = value["file_ext"].toString();

                m_songInfo.m_songAttrs << attr;
                m_songInfo.m_songName = MusicUtils::String::illegalCharactersReplaced(value["title"].toString());
                m_songInfo.m_singerName = MusicUtils::String::illegalCharactersReplaced(value["artist"].toString());
                m_songInfo.m_smallPicUrl = value["picture"].toString();
                m_songInfo.m_albumName = MusicUtils::String::illegalCharactersReplaced(value["albumtitle"].toString());
                m_songInfo.m_lrcUrl = "https://api.douban.com/v2/fm/lyric?sid=" + value["sid"].toString() + "&ssid=" + value["ssid"].toString();
                TTK_LOGGER_INFO(m_songInfo.m_lrcUrl);
            }
        }
    }

    Q_EMIT downLoadDataChanged("query finished!");
    deleteAll();
}
